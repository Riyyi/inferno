/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/application.h"
#include "inferno/asset/font.h"
#include "inferno/asset/texture.h"
#include "inferno/component/textareacomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/render/renderer.h"
#include "inferno/scene/scene.h"
#include "inferno/system/textareasystem.h"
#include "inferno/window.h"

namespace Inferno {

TextAreaSystem::TextAreaSystem(s)
{
	ruc::info("TextAreaSystem initialized");
}

TextAreaSystem::~TextAreaSystem()
{
}

void TextAreaSystem::render()
{
	auto view = m_scene->registry()->view<TransformComponent, TextAreaComponent>();

	glm::ivec2 viewport = {
		Application::the().window().getWidth(),
		Application::the().window().getHeight(),
	};

	for (auto [entity, transform, textarea] : view.each()) {
		// Loop through textareas content
		// Linebreak if width reached
		// Break if lines AND width reached
		// Calculate symbol quad
		// Submit symbol quad for rendering

		std::shared_ptr<Font> font = AssetManager::the().load<Font>(textarea.font);
		// glm::mat4 translate = transform.translate;

		m_symbols.clear();
		createLines(font, textarea);
		createQuads(font, textarea);
	}
}

using Symbols = std::vector<std::shared_ptr<Symbol>>;

void TextAreaSystem::createLines(std::shared_ptr<Font> font, const TextAreaComponent& textarea)
{
	float fontScale = textarea.fontSize / (float)font->size();

	// Texture
	// -------------------------------------

	float textureWidth = static_cast<float>(font->texture()->width());
	float textureHeight = static_cast<float>(font->texture()->height());
	VERIFY(textureWidth == textureHeight, "TextAreaSystem read invalid font texture");

	// -------------------------------------

	char previous = 0;
	size_t spaceIndex = 0;
	float lineWidth = 0.0f;
	float lineWidthSinceLastSpace = 0.0f;
	for (char symbol : textarea.content) {
		auto c = font->get(symbol);
		m_symbols.push_back(c);

		// Kerning
		char kerning = 0;
		if (c->kernings.find(previous) != c->kernings.end()) {
			kerning = c->kernings.at(previous);
		}

		lineWidth += (c->advance + kerning) * fontScale;
		lineWidthSinceLastSpace += (c->advance + kerning) * fontScale;

		if (symbol == ' ') {
			spaceIndex = m_symbols.size() - 1;
			lineWidthSinceLastSpace = 0;
		}

		if (lineWidth > textureWidth) {
			m_symbols[spaceIndex] = nullptr;
			lineWidth = 0;
			lineWidth = lineWidthSinceLastSpace;
		}

		previous = symbol;
	}
}

void TextAreaSystem::createQuads(std::shared_ptr<Font> font, const TextAreaComponent& textarea)
{
	float fontScale = textarea.fontSize / (float)font->size();

	char previous = 0;
	float advanceX = 0.0f;
	float advanceY = 0.0f;
	for (const auto& symbol : m_symbols) {
		// Go to the next line on "\n"
		if (symbol == nullptr) {
			advanceX = 0;
			advanceY -= (font->lineSpacing() * textarea.lineSpacing) * fontScale;
			continue;
		}

		std::optional<SymbolQuad> quad = calculateSymbolQuad(symbol, previous, font, fontScale, advanceX, advanceY);
		if (quad) {
			RendererFont::the().drawSymbol(quad.value(), font->texture());
		}

		previous = symbol->id;
	}
}

std::optional<SymbolQuad> TextAreaSystem::calculateSymbolQuad(std::shared_ptr<Symbol> c, char previous, std::shared_ptr<Font> font, float fontScale, float& advanceX, float& advanceY)
{
	SymbolQuad symbolQuad;

	// Texture
	// -------------------------------------

	float textureWidth = static_cast<float>(font->texture()->width());
	float textureHeight = static_cast<float>(font->texture()->height());
	VERIFY(textureWidth == textureHeight, "TextAreaSystem read invalid font texture");

	// Skip empty symbols (like space)
	if (c->size.x == 0 || c->size.y == 0) {
		// Jump to the next glyph
		advanceX += c->advance * fontScale;
		return {};
	}

	// Position
	// -------------------------------------

	// Kerning
	char kerning = 0;
	if (c->kernings.find(previous) != c->kernings.end()) {
		kerning = c->kernings.at(previous);
	}

	glm::vec2 cursor = { advanceX + (c->offset.x * fontScale) + (kerning * fontScale),
		                 advanceY - (c->offset.y * fontScale) };
	glm::vec2 cursorMax = { cursor.x + (c->size.x * fontScale),
		                    cursor.y - (c->size.y * fontScale) };

	// Scale the values from 0:512 (texture size) to -1:1 (screen space)
	glm::vec2 cursorScreen = {
		(cursor.x / textureWidth * 2) - 1,
		(cursor.y / textureHeight * 2) + 1,
	};
	glm::vec2 cursorScreenMax = {
		(cursorMax.x / textureWidth * 2) - 1,
		(cursorMax.y / textureHeight * 2) + 1,
	};

	symbolQuad.at(0).quad.position = { cursorScreen.x, cursorScreenMax.y, 0.0f };    // bottom left
	symbolQuad.at(1).quad.position = { cursorScreenMax.x, cursorScreenMax.y, 0.0f }; // bottom right
	symbolQuad.at(2).quad.position = { cursorScreenMax.x, cursorScreen.y, 0.0f };    // top right
	symbolQuad.at(3).quad.position = { cursorScreen.x, cursorScreen.y, 0.0f };       // top left

	// Jump to the next glyph
	advanceX += (c->advance + kerning) * fontScale;

	// Texture coordinates
	// -------------------------------------

	glm::vec2 x {
		1 - (textureWidth - c->position.x) / textureWidth,
		1 - (textureWidth - c->position.x - c->size.x) / textureWidth
	};
	glm::vec2 y {
		(textureHeight - c->position.y - c->size.y) / textureHeight,
		(textureHeight - c->position.y) / textureHeight
	};

	symbolQuad.at(0).quad.textureCoordinates = { x.x, y.x };
	symbolQuad.at(1).quad.textureCoordinates = { x.y, y.x };
	symbolQuad.at(2).quad.textureCoordinates = { x.y, y.y };
	symbolQuad.at(3).quad.textureCoordinates = { x.x, y.y };

	return symbolQuad;
}

} // namespace Inferno
