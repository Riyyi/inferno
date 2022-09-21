#pragma once

#include <cstdint> // uint32_t
#include <string>  // std::string
#include <utility> // std::move

#include "glm/ext/vector_float4.hpp" // glm::vec4

namespace Inferno {

	struct TextAreaComponent {
		std::string content { "" };
		std::string font { "" };
		uint32_t fontSize { 0 };
		uint32_t width { 0 };
		uint32_t lines { 0 };

		TextAreaComponent() {}
		TextAreaComponent(const std::string& content, const std::string& font,
		                  uint32_t fontSize, uint32_t width, uint32_t lines)
			: content(std::move(content)), font(std::move(font)), fontSize(fontSize), width(width), lines(lines) {}

		// booleans?
		// glm::vec4 outlineColor { 1.0f, 1.0f, 1.0f, 1.0f };
		// glow?
		// float dropShadow { 0.0f };
	};

} // namespace Inferno
