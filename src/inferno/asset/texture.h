/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint8_t, uint32_t
#include <memory>  // std::shared_ptr
#include <string_view>

#include "glad/glad.h"

#include "inferno/asset/asset-manager.h"

struct aiTexture;

namespace Inferno {

class Texture : public Asset {
public:
	virtual ~Texture();

	void init(uint32_t width, uint32_t height, uint8_t channels);
	void init(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t dataFormat, uint32_t dataType);

	virtual void bind(uint32_t unit = 0) const = 0;
	virtual void unbind() const = 0;

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t id() const { return m_id; }
	uint32_t internalFormat() const { return m_internalFormat; }
	uint32_t dataFormat() const { return m_dataFormat; }
	uint32_t dataType() const { return m_dataType; }

	virtual bool isTexture2D() const override { return false; }
	virtual bool isTextureCubemap() const override { return false; }
	virtual bool isTextureFramebuffer() const override { return false; }

protected:
	Texture(std::string_view path)
		: Asset(path)
	{
	}

protected:
	uint32_t m_width { 0 };
	uint32_t m_height { 0 };
	uint32_t m_id { 0 };
	uint32_t m_internalFormat { 0 };
	uint32_t m_dataFormat { 0 };
	uint32_t m_dataType { GL_UNSIGNED_BYTE };

private:
	virtual bool isTexture() const override { return true; }
};

// -------------------------------------

class Texture2D final : public Texture {
public:
	virtual ~Texture2D() = default;

	// Factory function
	static std::shared_ptr<Texture2D> create(std::string_view path);
	static std::shared_ptr<Texture2D> create(aiTexture* texture);

	virtual void bind(uint32_t unit = 0) const override;
	virtual void unbind() const override;

private:
	Texture2D(std::string_view path)
		: Texture(path)
	{
	}

	void createImpl(unsigned char* data);

	virtual bool isTexture2D() const override { return true; }
};

// -------------------------------------

class TextureCubemap final : public Texture {
public:
	virtual ~TextureCubemap() = default;

	// Factory function
	static std::shared_ptr<TextureCubemap> create(std::string_view path);

	virtual void bind(uint32_t unit = 0) const override;
	virtual void unbind() const override;

private:
	TextureCubemap(std::string_view path)
		: Texture(path)
	{
	}

	void createImpl();

	virtual bool isTextureCubemap() const override { return true; }
};

// -----------------------------------------

class TextureFramebuffer final : public Texture {
public:
	virtual ~TextureFramebuffer() = default;

	// Factory function
	static std::shared_ptr<TextureFramebuffer> create(
		std::string_view path,
		uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t dataFormat, uint32_t dataType = GL_UNSIGNED_BYTE);

	virtual void bind(uint32_t unit) const override;
	virtual void unbind() const override;

private:
	TextureFramebuffer(std::string_view path)
		: Texture(path)
	{
	}

	void createImpl();

	virtual bool isTextureFramebuffer() const override { return true; }
};

// -----------------------------------------

// clang-format off
template<>
inline bool Asset::fastIs<Texture>() const { return isTexture(); }

template<>
inline bool Asset::fastIs<Texture2D>() const { return isTexture2D(); }

template<>
inline bool Asset::fastIs<TextureCubemap>() const { return isTextureCubemap(); }

template<>
inline bool Asset::fastIs<TextureFramebuffer>() const { return isTextureFramebuffer(); }
// clang-format on

} // namespace Inferno
