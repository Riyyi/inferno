/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>       // uint8_t, uint32_t
#include <memory>        // std::shared_ptr
#include <string>        // std::string
#include <unordered_map> // std::unordered_map

#include "ruc/singleton.h"

namespace Inferno {

class Texture2D;
class TextureCubemap;

class Texture {
public:
	virtual ~Texture();

	enum Type : uint8_t {
		TwoDimensional = 0,
		Cubemap,
	};

	void init(uint32_t width, uint32_t height, uint8_t channels);

	virtual void bind(uint32_t unit = 0) const = 0;
	virtual void unbind() const = 0;

	std::string path() const { return m_path; }
	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t id() const { return m_id; }
	uint32_t internalFormat() const { return m_internalFormat; }
	uint32_t dataFormat() const { return m_dataFormat; }

	virtual bool is2D() const { return false; }
	virtual bool isCubeMap() const { return false; }

	friend Texture2D;
	friend TextureCubemap;

protected:
	Texture() {}

protected:
	std::string m_path;
	uint32_t m_width { 0 };
	uint32_t m_height { 0 };
	uint32_t m_id { 0 };
	uint32_t m_internalFormat { 0 };
	uint32_t m_dataFormat { 0 };
};

// -------------------------------------

class Texture2D final : public Texture {
public:
	virtual ~Texture2D() = default;

	// Factory function
	static std::shared_ptr<Texture> create(const std::string& path);

	virtual void bind(uint32_t unit = 0) const override;
	virtual void unbind() const override;

private:
	Texture2D() {}

	virtual bool is2D() const override { return true; }

private:
	void create(unsigned char* data);
};

// -------------------------------------

class TextureCubemap final : public Texture {
public:
	virtual ~TextureCubemap() = default;

	// Factory function
	static std::shared_ptr<Texture> create(const std::string& path);

	virtual void bind(uint32_t unit = 0) const override;
	virtual void unbind() const override;

private:
	TextureCubemap() {};

	virtual bool isCubeMap() const override { return true; }

private:
	void create();
};

// -------------------------------------

class TextureManager final : public ruc::Singleton<TextureManager> {
public:
	TextureManager(s);
	~TextureManager();

	void add(const std::string& path, std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> load(const std::string& path, Texture::Type type = Texture::Type::TwoDimensional);
	std::shared_ptr<Texture> get(const std::string& path);
	bool exists(const std::string& path);

	void remove(const std::string& path);
	void remove(std::shared_ptr<Texture> texture);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureList;
};

} // namespace Inferno
