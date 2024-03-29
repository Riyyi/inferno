/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <climits> // UINT_MAX
#include <cstdint> // uint8_t, uint32_t
#include <memory>  // std::shared_ptr
#include <utility> // std::move

#include "glad/glad.h"
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "inferno/render/texture.h"

namespace Inferno {

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::init(uint32_t width, uint32_t height, uint8_t channels)
{
	m_width = width;
	m_height = height;

	m_internalFormat = (channels == 3) ? GL_RGB8 : GL_RGBA8;
	m_dataFormat = (channels == 3) ? GL_RGB : GL_RGBA;
}

// -----------------------------------------

std::shared_ptr<Texture> Texture2D::create(const std::string& path)
{
	auto result = std::shared_ptr<Texture2D>(new Texture2D);
	result->m_path = path;

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = nullptr;

	// Load image data
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);
	VERIFY(data, "failed to load image: '{}'", path);

	result->init(width, height, channels);
	result->create(data);

	// Clean resources
	stbi_image_free(data);

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

void Texture2D::create(unsigned char* data)
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
		GL_UNSIGNED_BYTE,  // Texture source datatype
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

std::shared_ptr<Texture> TextureCubemap::create(const std::string& path)
{
	auto result = std::shared_ptr<TextureCubemap>(new TextureCubemap);
	result->m_path = path;

	result->create();

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

void TextureCubemap::create()
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
			GL_UNSIGNED_BYTE,                   // Texture source datatype
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

TextureManager::TextureManager(s)
{
	ruc::info("TextureManager initialized");
}

TextureManager::~TextureManager()
{
}

void TextureManager::add(const std::string& path, std::shared_ptr<Texture> texture)
{
	// Construct (key, value) pair and insert it into the unordered_map
	m_textureList.emplace(std::move(path), std::move(texture));
}

std::shared_ptr<Texture> TextureManager::load(const std::string& path, Texture::Type type)
{
	if (exists(path)) {
		return get(path);
	}

	auto texture = (type == Texture::TwoDimensional) ? Texture2D::create(path) : TextureCubemap::create(path);
	add(path, texture);

	return texture;
}

std::shared_ptr<Texture> TextureManager::get(const std::string& path)
{
	return exists(path) ? m_textureList.at(path) : nullptr;
}

bool TextureManager::exists(const std::string& path)
{
	return m_textureList.find(path) != m_textureList.end();
}

void TextureManager::remove(const std::string& path)
{
	if (exists(path)) {
		m_textureList.erase(path);
	}
}

void TextureManager::remove(std::shared_ptr<Texture> texture)
{
	if (exists(texture->path())) {
		m_textureList.erase(texture->path());
	}
}

} // namespace Inferno
