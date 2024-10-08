/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <climits> // UINT_MAX
#include <cstdint> // uint8_t, uint32_t
#include <memory>  // std::shared_ptr

#include "assimp/texture.h"
#include "glad/glad.h"
#include "ruc/meta/assert.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "inferno/asset/texture.h"

namespace Inferno {

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

// -----------------------------------------

void Texture::savePNG(std::string_view path, std::shared_ptr<Texture> texture)
{
	texture->bind();

	// Allocate memory
	uint32_t dataFormat = texture->m_dataFormat == GL_RGBA ? 4 : 3;
	size_t dataSize = texture->m_width * texture->m_height * dataFormat;
	std::vector<unsigned char> data(dataSize);

	// Read texture data from the GPU
	glGetTexImage(GL_TEXTURE_2D, 0, texture->m_dataFormat, texture->m_dataType, data.data());

	// Write image to a file
	stbi_flip_vertically_on_write(1);
	stbi_write_png(
		path.data(),
		texture->m_width,
		texture->m_height,
		dataFormat,
		data.data(),
		texture->m_width * dataFormat);

	texture->unbind();
}

void Texture::saveScreenshotPNG(std::string_view path, uint32_t width, uint32_t height)
{
	// Allocate memory
	size_t dataSize = width * height * 4;
	std::vector<unsigned char> data(dataSize);

	// Set the pack alignment to 1 (to avoid row alignment issues)
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Read the pixels from the default framebuffer
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

	stbi_flip_vertically_on_write(1);
	stbi_write_png(path.data(), width, height, 4, data.data(), width * 4);
}

// -----------------------------------------

void Texture::init(uint32_t width, uint32_t height, uint8_t channels)
{
	init(width, height,
	     (channels == 3) ? GL_RGB8 : GL_RGBA8,
	     (channels == 3) ? GL_RGB : GL_RGBA,
	     GL_UNSIGNED_BYTE);
}

void Texture::init(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t dataFormat, uint32_t dataType)
{
	m_width = width;
	m_height = height;
	m_internalFormat = internalFormat;
	m_dataFormat = dataFormat;
	m_dataType = dataType;
}

// -----------------------------------------

std::shared_ptr<Texture2D> Texture2D::create(std::string_view path)
{
	auto result = std::shared_ptr<Texture2D>(new Texture2D(path));

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = nullptr;

	// Load image data
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.data(), &width, &height, &channels, STBI_default);
	VERIFY(data, "failed to load image: '{}'", path);

	result->init(width, height, channels);
	result->createImpl(data);

	// Clean resources
	stbi_image_free(data);

	return result;
}

std::shared_ptr<Texture2D> Texture2D::create(aiTexture* texture)
{
	auto result = std::shared_ptr<Texture2D>(new Texture2D(texture->mFilename.C_Str()));

	int width = static_cast<int>(texture->mWidth);
	int height = static_cast<int>(texture->mHeight);
	int channels = 0;
	unsigned char* data = reinterpret_cast<unsigned char*>(texture->pcData);
	bool isCompressed = height == 0; // height 0 is compression, byte length stored in width variable

	if (isCompressed) {
		stbi_set_flip_vertically_on_load(0);
		data = stbi_load_from_memory(data, width, &width, &height, &channels, STBI_default);
	}
	else {
		channels = 4; // ARGB888
		// TODO: imprement format hints? `archFormatHint'
		VERIFY_NOT_REACHED();
	}

	result->init(width, height, channels);
	result->createImpl(data);

	return result;
}

void Texture2D::bind(uint32_t unit) const
{
	// Set active unit
	glActiveTexture(GL_TEXTURE0 + unit);

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Reset unit
	glActiveTexture(GL_TEXTURE0);
}

void Texture2D::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::createImpl(unsigned char* data)
{
	m_id = UINT_MAX;

	// Create texture object
	glGenTextures(1, &m_id);

	// Bind texture object
	glBindTexture(GL_TEXTURE_2D, m_id);

	// Set unpacking of pixel data to byte-alignment,
	// this prevents alignment issues when using a single byte for color
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate texture
	glTexImage2D(
		GL_TEXTURE_2D,     // Texture target
		0,                 // Midmap level, base starts at level 0
		m_internalFormat,  // Texture format
		m_width, m_height, // Image width/height
		0,                 // Always 0 (legacy)
		m_dataFormat,      // Texture source format
		m_dataType,        // Texture source datatype
		data);             // Image data

	// Set the texture wrapping / filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Magnify
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Minify
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);      // X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);      // Y

	// Automatically generate all mipmap levels
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture object
	glBindTexture(GL_TEXTURE_2D, 0);
}

// -----------------------------------------

std::shared_ptr<TextureCubemap> TextureCubemap::create(std::string_view path)
{
	auto result = std::shared_ptr<TextureCubemap>(new TextureCubemap(path));

	result->createImpl();

	return result;
}

void TextureCubemap::bind(uint32_t unit) const
{
	// Set active unit
	glActiveTexture(GL_TEXTURE0 + unit);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	// Reset unit
	glActiveTexture(GL_TEXTURE0);
}

void TextureCubemap::unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubemap::createImpl()
{
	m_id = UINT_MAX;

	// Create texture object
	glGenTextures(1, &m_id);

	// Bind texture object
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	// Set unpacking of pixel data to byte-alignment,
	// this prevents alignment issues when using a single byte for color
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	stbi_set_flip_vertically_on_load(0);

	static constexpr const char* cubemapPaths[6] { "-px", "-nx", "-py", "-ny", "-pz", "-nz" };
	size_t dotIndex = m_path.find_last_of('.');
	std::string path = m_path.substr(0, dotIndex);
	std::string extension = m_path.substr(dotIndex);

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = nullptr;
	for (size_t i = 0; i < 6; ++i) {
		std::string facePath = path + cubemapPaths[i] + extension;

		// Load image data
		data = stbi_load(facePath.c_str(), &width, &height, &channels, STBI_default);
		VERIFY(data, "failed to load image: '{}'", facePath.c_str());

		init(width, height, channels);

		// Generate texture face
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Texture target
			0,                                  // Midmap level, base starts at level 0
			m_internalFormat,                   // Internal format
			m_width, m_height,                  // Image width/height
			0,                                  // Always 0 (legacy)
			m_dataFormat,                       // Texture source format
			m_dataType,                         // Texture source datatype
			data);                              // Image data

		// Clean resources
		stbi_image_free(data);
	}

	// Set the texture wrapping / filtering options
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // Magnify
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // Minify
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // X
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Y
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Z

	// Unbind texture object
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// -----------------------------------------

std::shared_ptr<TextureFramebuffer> TextureFramebuffer::create(
	std::string_view path,
	uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t dataFormat, uint32_t dataType)
{
	auto result = std::shared_ptr<TextureFramebuffer>(new TextureFramebuffer(path));

	result->init(width, height, internalFormat, dataFormat, dataType);
	result->createImpl();

	return result;
}

void TextureFramebuffer::bind(uint32_t unit) const
{
	// Set active unit
	glActiveTexture(GL_TEXTURE0 + unit);

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Reset unit
	glActiveTexture(GL_TEXTURE0);
}

void TextureFramebuffer::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureFramebuffer::createImpl()
{
	m_id = UINT_MAX;

	// Create texture object
	glGenTextures(1, &m_id);

	// Bind texture object
	glBindTexture(GL_TEXTURE_2D, m_id);

	// Generate texture
	glTexImage2D(
		GL_TEXTURE_2D,     // Texture target
		0,                 // Midmap level, base starts at level 0
		m_internalFormat,  // Texture format
		m_width, m_height, // Image width/height
		0,                 // Always 0 (legacy)
		m_dataFormat,      // Texture source format
		m_dataType,        // Texture source datatype
		NULL);             // Image data

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Unbind texture object
	glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Inferno
