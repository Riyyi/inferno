/*
 * Copyright (C) 2022 Riyyi
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

class Texture {
public:
	Texture(const std::string& path);
	Texture(unsigned char* data, uint32_t width, uint32_t height, uint8_t channels = 3);
	virtual ~Texture();

	void bind(uint32_t unit = 0) const;
	void unbind() const;

	inline std::string path() const { return m_path; }
	inline uint32_t width() const { return m_width; }
	inline uint32_t height() const { return m_height; }
	inline uint32_t id() const { return m_id; }
	inline uint32_t internalFormat() const { return m_internalFormat; }
	inline uint32_t dataFormat() const { return m_dataFormat; }

protected:
	void init(unsigned char* data, uint32_t width, uint32_t height, uint8_t channels);
	void create(unsigned char* data);

private:
	std::string m_path;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_id;
	uint32_t m_internalFormat;
	uint32_t m_dataFormat;
};

// -------------------------------------

class TextureManager final : public ruc::Singleton<TextureManager> {
public:
	TextureManager(s);
	~TextureManager();

	void add(const std::string& path, std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> load(const std::string& path);
	std::shared_ptr<Texture> get(const std::string& path);
	bool exists(const std::string& path);

	void remove(const std::string& path);
	void remove(std::shared_ptr<Texture> texture);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureList;
};

} // namespace Inferno
