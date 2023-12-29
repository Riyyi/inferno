/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>  // std::uint32_t
#include <memory>   // std::shared_ptr
#include <optional> // std::optional
#include <vector>   // std::vector

#include "glm/ext/vector_float3.hpp" // glm::vec3
#include "ruc/singleton.h"

#include "inferno/component/textareacomponent.h"
#include "inferno/render/font.h"
#include "inferno/render/renderer.h"

namespace Inferno {

using Characters = std::vector<std::shared_ptr<Character>>;
using CharacterQuad = std::array<CharacterVertex, Renderer::vertexPerQuad>;

class Font;
class Scene;

class TextAreaSystem final : public ruc::Singleton<TextAreaSystem> {
public:
	TextAreaSystem(s);
	virtual ~TextAreaSystem();

	void render();

	void setScene(Scene* scene) { m_scene = scene; }

private:
	void createLines(std::shared_ptr<Font> font, const TextAreaComponent& textarea);
	void createQuads(std::shared_ptr<Font> font, const TextAreaComponent& textarea);

	std::optional<CharacterQuad> calculateCharacterQuad(std::shared_ptr<Character> c, char previous, std::shared_ptr<Font> font, float fontSize, float& advanceX, float& advanceY);

	Characters m_characters;
	Scene* m_scene { nullptr };
};

} // namespace Inferno
