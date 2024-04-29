/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>   // std::shared_ptr
#include <optional> // std::optional
#include <vector>   // std::vector

#include "ruc/singleton.h"

#include "inferno/asset/font.h"
#include "inferno/render/renderer.h"

namespace Inferno {

using Symbols = std::vector<std::shared_ptr<Symbol>>;
using SymbolQuad = std::array<SymbolVertex, Renderer<void>::vertexPerQuad>;

class Scene;
class TextAreaComponent;

class TextAreaSystem final : public ruc::Singleton<TextAreaSystem> {
public:
	TextAreaSystem(s);
	virtual ~TextAreaSystem();

	void render();

	void setScene(Scene* scene) { m_scene = scene; }

private:
	void createLines(std::shared_ptr<Font> font, const TextAreaComponent& textarea);
	void createQuads(std::shared_ptr<Font> font, const TextAreaComponent& textarea);

	std::optional<SymbolQuad> calculateSymbolQuad(std::shared_ptr<Symbol> c, char previous, std::shared_ptr<Font> font, float fontSize, float& advanceX, float& advanceY);

	Symbols m_symbols;
	Scene* m_scene { nullptr };
};

} // namespace Inferno
