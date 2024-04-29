/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <array>    // std::array
#include <charconv> // std;:from_chars
#include <cstdint>  // int32_t, uint32_t
#include <ranges>   // std::views::split
#include <string>   // std::getline

#include "ruc/file.h"
#include "ruc/meta/assert.h"
#include "ruc/meta/concepts.h"

#include "inferno/asset/font.h"
#include "inferno/asset/texture.h"

namespace Inferno {

std::shared_ptr<Font> Font::create(std::string_view path)
{
	auto result = std::shared_ptr<Font>(new Font(path));

	auto stringPath = std::string(path);
	std::string file = stringPath + ".fnt";
	std::string image = stringPath + ".png";

	std::string font = ruc::File(file).data();
	result->parseFont(font);

	result->m_texture = Texture2D::create(image);

	return result;
}

// TODO: Move this to ruc
template<Integral T>
static T convert(std::string_view value)
{
	T tmp;
	std::from_chars(value.data(), value.data() + value.size(), tmp);
	return tmp;
}

void Font::parseFont(const std::string& font)
{
	std::istringstream iss(font);
	for (std::string line; std::getline(iss, line);) {
		std::string action = findAction(line);
		const std::vector<std::string> columns = findColumns(line);

		// Info
		// ---------------------------------

		if (action.compare("info") == 0) {
			m_size = convert<unsigned char>(findValue("size", columns));
			auto paddings = findValue("padding", columns) | std::views::split(',');
			size_t i = 0;
			for (const auto& padding : paddings) {
				// top, right, bottom, left
				m_padding[i++] = convert<uint32_t>(padding.data()) - PADDING;
			}
			continue;
		}

		// Common
		// ---------------------------------

		if (action.compare("common") == 0) {
			m_lineSpacing = convert<uint32_t>(findValue("lineHeight", columns)) - m_padding[Padding::Top] - m_padding[Padding::Bottom];
			continue;
		}

		// Symbol
		// ---------------------------------

		if (action.compare("char") == 0) {
			auto id = convert<char>(findValue("id", columns));
			auto width = convert<uint32_t>(findValue("width", columns));
			auto height = convert<uint32_t>(findValue("height", columns));
			Symbol symbol = {
				.id = id,
				.position = {
					convert<uint32_t>(findValue("x", columns)) + m_padding[Padding::Left],
					convert<uint32_t>(findValue("y", columns)) + m_padding[Padding::Top],
				},
				.size = {
					width == 0 ? 0 : width - m_padding[Padding::Left] - m_padding[Padding::Right],
					height == 0 ? 0 : height - m_padding[Padding::Top] - m_padding[Padding::Bottom],
				},
				.offset = {
					convert<int32_t>(findValue("xoffset", columns)) + m_padding[Padding::Left],
					convert<int32_t>(findValue("yoffset", columns)) + m_padding[Padding::Top],
				},
				.advance = convert<uint32_t>(findValue("xadvance", columns)) - m_padding[Padding::Left] - m_padding[Padding::Right]
			};

			m_symbolList.emplace(id, std::make_shared<Symbol>(symbol));
			continue;
		}

		// Kerning
		// ---------------------------------

		if (action.compare("kerning") == 0) {
			unsigned char first = convert<unsigned char>(findValue("first", columns));
			unsigned char second = convert<unsigned char>(findValue("second", columns));
			char amount = convert<char>(findValue("amount", columns));

			// Add the kerning of the previous symbol to this symbol
			if (m_symbolList.find(second) != m_symbolList.end()) {
				auto symbol = m_symbolList.at(second);
				symbol->kernings.emplace(first, amount);
			}

			continue;
		}
	}
}

std::string Font::findAction(const std::string& line) const
{
	return line.substr(0, line.find(" "));
}

std::vector<std::string> Font::findColumns(const std::string& line) const
{
	std::vector<std::string> elements;

	size_t index = 0;
	size_t find = 0;
	size_t findFirstNotOf = 0;
	// Loop over line symbols
	while (find != std::string::npos) {
		find = line.find(" ", index);
		findFirstNotOf = line.find_first_not_of(" ", index);

		// Add element
		if (find > findFirstNotOf) {
			elements.push_back(line.substr(index, find - findFirstNotOf));
			index += 1 + find - findFirstNotOf;
		}
		// Skip double spaces
		else {
			index = findFirstNotOf;
		}
	}

	VERIFY(!elements.empty(), "Font file did not find any columns");
	return elements;
}

std::string Font::findValue(const std::string& key, const std::vector<std::string>& columns) const
{
	size_t find = 0;
	// Loop over columns
	for (auto& column : columns) {
		find = column.find(key + "=");
		if (find != std::string::npos) {
			return column.substr(key.length() + 1);
		}
	}

	VERIFY(false, "Font file did not contain key '{}'", key);
	return "";
}

} // namespace Inferno

void ruc::format::Formatter<glm::ivec2>::format(Builder& builder, glm::ivec2 value) const
{
	return Formatter<std::vector<int32_t>>::format(builder, { value.x, value.y });
}
