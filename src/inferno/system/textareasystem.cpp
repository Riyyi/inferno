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

		unsigned char previous = 0;
		float advanceX = 0.0f;
		float advanceY = 0.0f;
		for (auto character : textarea.content) {
			std::optional<CharacterQuad> quad = calculateCharacterQuad(character, previous, font, advanceX, advanceY);
			previous = character;

			if (quad) {
				RendererCharacter::the().drawCharacter(quad.value(), font->texture());
			}
		}
	}
}

std::optional<CharacterQuad> TextAreaSystem::calculateCharacterQuad(unsigned char character, unsigned char previous, std::shared_ptr<Font> font, float& advanceX, float& advanceY)
{
	CharacterQuad characterQuad;

	auto c = font->get(character);

	// Texture
	// -------------------------------------

	float textureWidth = static_cast<float>(font->texture()->width());
	float textureHeight = static_cast<float>(font->texture()->height());
	VERIFY(textureWidth == textureHeight, "TextAreaSystem read invalid font texture");

	// Skip empty characters (like space)
	if (c->size.x == 0 || c->size.y == 0) {
		// Jump to the next glyph
		advanceX += c->advance;
		return {};
	}

	// Position
	// -------------------------------------

	// Kerning
	if (c->kernings.find(previous) != c->kernings.end()) {
		advanceX += c->kernings.at(previous);
	}

	// Line wrapping
	if (advanceX + c->offset.x + c->size.x > textureWidth) {
		advanceX = 0;
		advanceY -= font->lineSpacing();
	}

	glm::vec2 cursor = { std::max(advanceX + c->offset.x, 0.0f),
		                 advanceY - c->offset.y };
	glm::vec2 cursorMax = { cursor.x + c->size.x,
		                    cursor.y - c->size.y };

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
	advanceX += c->advance;

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
