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
		info() << "TextAreaSystem initialized";
	}

	TextAreaSystem::~TextAreaSystem()
	{
	}

	void TextAreaSystem::render()
	{
		auto view = m_scene->registry()->view<TransformComponent, TextAreaComponent>();

		glm::ivec2 viewport = {
			Application::the().getWindow().getWidth(),
			Application::the().getWindow().getHeight(),
		};

		for (auto [entity, transform, textarea] : view.each()) {
			// Loop through textareas content
			// Linebreak if width reached
			// Break if lines AND width reached
			// Calculate character quad
			// Submit character quad for rendering

			std::shared_ptr<Font> font = FontManager::the().load(textarea.font);
			// glm::mat4 translate = transform.translate;

			float advance = 0.0f;
			for (auto character : textarea.content) {
				std::optional<CharacterQuad> quad = calculateCharacterQuad(character, font, advance);

				if (quad) {
					RendererCharacter::the().drawCharacter(quad.value(), font->texture());
				}
			}
		}
	}

	std::optional<CharacterQuad> TextAreaSystem::calculateCharacterQuad(unsigned char character, std::shared_ptr<Font> font, float& advance)
	{
		CharacterQuad characterQuad;

		auto c = font->get(character);

		// Texture
		// ---------------------------------

		float textureWidth = static_cast<float>(font->texture()->width());
		float textureHeight = static_cast<float>(font->texture()->height());
		VERIFY(textureWidth == textureHeight, "TextAreaSystem read invalid font texture");

		// Skip empty characters
		if (c->size.x == 0 || c->size.y == 0) {
			// Jump to the next glyph
			advance += c->advance / textureWidth;
			return {};
		}

		// Position
		// ---------------------------------

		float quadWidth  = c->size.x / textureWidth;
		float quadHeight = c->size.y / textureHeight;
		characterQuad.at(0).quad.position = { -quadWidth, -quadHeight, 0.0f }; // bottom left
		characterQuad.at(1).quad.position = {  quadWidth, -quadHeight, 0.0f }; // bottom right
		characterQuad.at(2).quad.position = {  quadWidth,  quadHeight, 0.0f }; // top right
		characterQuad.at(3).quad.position = { -quadWidth,  quadHeight, 0.0f }; // top left

		for (auto& quad : characterQuad) {
			quad.quad.position.x -= 0.5f;

			quad.quad.position.x += c->offset.x / (float)textureWidth;
			quad.quad.position.y -= c->offset.y / (float)textureHeight;

			quad.quad.position.x += advance;
		}

		// dbgln("character: {} ({}) width: {} height: {} advance: {} x: {}",
		//       character, (int)character, quadWidth, quadHeight, advance, characterQuad.at(0).quad.position.x);

		// Jump to the next glyph
		advance += c->advance / textureWidth;

		// Texture coordinates
		// ---------------------------------

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
