#include <climits> // UINT_MAX
#include <memory>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "inferno/assertions.h"
#include "inferno/render/texture.h"

namespace Inferno {

	Texture::Texture(const std::string& path)
	{
		int width;
		int height;
		int channels;

		// Load image data
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);

		ASSERT(data, "Failed to load image: '{}'", path);

		m_width = width;
		m_height = height;

		if (channels == 4) {
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		create(data);

		// Clean resources
		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
	}

	void Texture::bind(uint32_t unit) const
	{
		// Set active unit
		glActiveTexture(GL_TEXTURE0 + unit);

		glBindTexture(GL_TEXTURE_2D, m_id);

		// Reset unit
		glActiveTexture(GL_TEXTURE0);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::create(unsigned char* data)
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // X
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Y
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Minify
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Magnify

		// Automatically generate all mipmap levels
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind texture object
		glBindTexture(GL_TEXTURE_2D, 0);
	}

// -----------------------------------------

	TextureManager* TextureManager::s_instance = nullptr;

	void TextureManager::initialize()
	{
		ASSERT(!s_instance, "TextureManager already exists!");
		s_instance = this;

		dbg(Log::Info) << "TextureManager initialized";
	}

	void TextureManager::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	void TextureManager::add(const std::string& path, const std::shared_ptr<Texture>& texture)
	{
		// Construct (key, value) pair and insert it into the unordered_map
		m_textureList.emplace(path, texture);
	}

	std::shared_ptr<Texture> TextureManager::load(const std::string& path)
	{
		if (exists(path)) {
			return get(path);
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
		add(path, texture);
		return get(path);
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

	void TextureManager::remove(const std::shared_ptr<Texture>& texture)
	{
		if (exists(texture->path())) {
			m_textureList.erase(texture->path());
		}
	}

}
