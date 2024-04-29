/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>   // std::array
#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr
#include <string>  // std::string
#include <string_view>
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include "glm/ext/vector_int2.hpp"  // glm::ivec2
#include "glm/ext/vector_uint2.hpp" // glm::uvec2
#include "ruc/format/format.h"

#include "inferno/asset/asset-manager.h"

#define PADDING 3

namespace Inferno {

class Texture;

struct Symbol {
	char id;                                          // Symbol
	glm::uvec2 position;                              // Position
	glm::uvec2 size;                                  // Width/height
	glm::ivec2 offset;                                // Offset from baseline to left / top of glyph
	uint32_t advance;                                 // Amount to advance to next glyph
	std::unordered_map<unsigned char, char> kernings; // Kernings for symbols that come before this one
};

// -------------------------------------

class Font final : public Asset {
public:
	virtual ~Font() {}

	// Factory function
	static std::shared_ptr<Font> create(std::string_view path);

	enum Padding {
		Top = 0,
		Right,
		Bottom,
		Left,
	};

	inline uint32_t size() const { return m_size; }
	inline uint32_t lineSpacing() const { return m_lineSpacing; }
	inline std::shared_ptr<Texture> texture() const { return m_texture; }

	inline std::shared_ptr<Symbol> get(unsigned char c) const { return m_symbolList.at(c); }
	inline std::shared_ptr<Symbol> operator[](unsigned char c) const { return m_symbolList.at(c); }

private:
	Font(std::string_view path)
		: Asset(path)
	{
	}

	void parseFont(const std::string& font);
	std::string findAction(const std::string& line) const;
	std::vector<std::string> findColumns(const std::string& line) const;
	std::string findValue(const std::string& key, const std::vector<std::string>& columns) const;

	virtual bool isFont() const override { return true; }

private:
	unsigned char m_size = { 0 };
	uint32_t m_lineSpacing = { 0 };
	std::array<uint32_t, 4> m_padding = { 0 };
	std::shared_ptr<Texture> m_texture;
	std::unordered_map<unsigned char, std::shared_ptr<Symbol>> m_symbolList;
};

// -----------------------------------------

// clang-format off
template<>
inline bool Asset::fastIs<Font>() const { return isFont(); }
// clang-format on

} // namespace Inferno

template<>
struct ruc::format::Formatter<glm::ivec2> : Formatter<std::vector<int32_t>> {
	void format(Builder& builder, glm::ivec2 value) const;
};

// FontManager fm;
// Font f = fm.load("path/to/font");
// Font f2("path/to/font");
// Symbol c = f['a'];

// Look into using signed distance fields for texture map generation ? anti-aliasing for text
// https://youtu.be/d8cfgcJR9Tk
