/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // std::max

#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/application.h"
#include "inferno/component/textareacomponent.h"
#include "inferno/render/font.h"
#include "inferno/render/renderer.h"
#include "inferno/render/texture.h"
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
		// Calculate character quad
		// Submit character quad for rendering

		std::shared_ptr<Font> font = FontManager::the().load(textarea.font);
		// glm::mat4 translate = transform.translate;

		m_characters.clear();
		createLines(font, textarea);
		createQuads(font, textarea);
	}
}

using Characters = std::vector<std::shared_ptr<Character>>;

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
	for (char character : textarea.content) {
		auto c = font->get(character);
		m_characters.push_back(c);

		// Kerning
		char kerning = 0;
		if (c->kernings.find(previous) != c->kernings.end()) {
			kerning = c->kernings.at(previous);
		}

		lineWidth += (c->advance + kerning) * fontScale;
		lineWidthSinceLastSpace += (c->advance + kerning) * fontScale;

		if (character == ' ') {
			spaceIndex = m_characters.size() - 1;
			lineWidthSinceLastSpace = 0;
		}

		if (lineWidth > textureWidth) {
			m_characters[spaceIndex] = nullptr;
			lineWidth = 0;
			lineWidth = lineWidthSinceLastSpace;
		}

		previous = character;
	}
}

void TextAreaSystem::createQuads(std::shared_ptr<Font> font, const TextAreaComponent& textarea)
{
	float fontScale = textarea.fontSize / (float)font->size();

	char previous = 0;
	float advanceX = 0.0f;
	float advanceY = 0.0f;
	for (const auto& character : m_characters) {
		// Go to the next line on "\n"
		if (character == nullptr) {
			advanceX = 0;
			advanceY -= (font->lineSpacing() * textarea.lineSpacing) * fontScale;
			continue;
		}

		std::optional<CharacterQuad> quad = calculateCharacterQuad(character, previous, font, fontScale, advanceX, advanceY);
		if (quad) {
			RendererCharacter::the().drawCharacter(quad.value(), font->texture());
		}

		previous = character->id;
	}
}

std::optional<CharacterQuad> TextAreaSystem::calculateCharacterQuad(std::shared_ptr<Character> c, char previous, std::shared_ptr<Font> font, float fontScale, float& advanceX, float& advanceY)
{
	CharacterQuad characterQuad;

	// Texture
	// -------------------------------------

	float textureWidth = static_cast<float>(font->texture()->width());
	float textureHeight = static_cast<float>(font->texture()->height());
	VERIFY(textureWidth == textureHeight, "TextAreaSystem read invalid font texture");

	// Skip empty characters (like space)
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

	characterQuad.at(0).quad.position = { cursorScreen.x, cursorScreenMax.y, 0.0f };    // bottom left
	characterQuad.at(1).quad.position = { cursorScreenMax.x, cursorScreenMax.y, 0.0f }; // bottom right
	characterQuad.at(2).quad.position = { cursorScreenMax.x, cursorScreen.y, 0.0f };    // top right
	characterQuad.at(3).quad.position = { cursorScreen.x, cursorScreen.y, 0.0f };       // top left

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

	characterQuad.at(0).quad.textureCoordinates = { x.x, y.x };
	characterQuad.at(1).quad.textureCoordinates = { x.y, y.x };
	characterQuad.at(2).quad.textureCoordinates = { x.y, y.y };
	characterQuad.at(3).quad.textureCoordinates = { x.x, y.y };

	return characterQuad;
}

} // namespace Inferno
