/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint8_t, uint32_t
#include <memory>  // std::shared_ptr
#include <string_view>

#include "inferno/asset/asset-manager.h"

namespace Inferno {

class Texture2D;
class TextureCubemap;

class Texture : public Asset {
public:
	virtual ~Texture();

	void init(uint32_t width, uint32_t height, uint8_t channels);

	virtual void bind(uint32_t unit = 0) const = 0;
	virtual void unbind() const = 0;

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t id() const { return m_id; }
	uint32_t internalFormat() const { return m_internalFormat; }
	uint32_t dataFormat() const { return m_dataFormat; }

	virtual bool is2D() const { return false; }
	virtual bool isCubemap() const { return false; }

	friend Texture2D;
	friend TextureCubemap;

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

private:
	virtual bool isTexture() const override { return true; }
};

// -------------------------------------

class Texture2D final : public Texture {
public:
	virtual ~Texture2D() = default;

	// Factory function
	static std::shared_ptr<Texture2D> create(std::string_view path);

	virtual void bind(uint32_t unit = 0) const override;
	virtual void unbind() const override;

private:
	Texture2D(std::string_view path)
		: Texture(path)
	{
	}

	virtual bool isTexture2D() const override { return true; }

private:
	void create(unsigned char* data);
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

	virtual bool isTextureCubemap() const override { return true; }

private:
	void create();
};

// -----------------------------------------

// clang-format off
template<>
inline bool Asset::fastIs<Texture>() const { return isTexture(); }

template<>
inline bool Asset::fastIs<Texture2D>() const { return isTexture2D(); }

template<>
inline bool Asset::fastIs<TextureCubemap>() const { return isTextureCubemap(); }
// clang-format on

} // namespace Inferno
