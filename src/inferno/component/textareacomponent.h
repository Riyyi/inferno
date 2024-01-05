/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <string>  // std::string
#include <utility> // std::move

#include "glm/ext/vector_float4.hpp" // glm::vec4
#include "ruc/json/json.h"

namespace Inferno {

struct TextAreaComponent {
	std::string content;
	std::string font;
	unsigned char fontSize { 12 };
	float lineSpacing { 1.0f };
	uint32_t width { 0 };

#if 0
	TextAreaComponent() {}
	TextAreaComponent(const std::string& content, const std::string& font,
	                  uint32_t fontSize, uint32_t width, uint32_t lines)
		: content(std::move(content))
		, font(std::move(font))
		, fontSize(fontSize)
		, width(width)
		, lines(lines)
	{
	}
#endif

	// booleans?
	// glm::vec4 outlineColor { 1.0f, 1.0f, 1.0f, 1.0f };
	// glow?
	// float dropShadow { 0.0f };
};

void fromJson(const ruc::Json& json, TextAreaComponent& value);

} // namespace Inferno
