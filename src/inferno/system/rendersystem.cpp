/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::radians
#include "ruc/format/log.h"

#include "inferno/component/cubemap-component.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/render/renderer.h"
#include "inferno/system/rendersystem.h"

namespace Inferno {

RenderSystem::RenderSystem(s)
{
	ruc::info("RenderSystem initialized");
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::render()
{
	auto quadView = m_registry->view<TransformComponent, SpriteComponent>();

	for (auto [entity, transform, sprite] : quadView.each()) {
		Renderer2D::the().drawQuad(transform, sprite.color, sprite.texture);
	}

	auto cubemapView = m_registry->view<TransformComponent, CubemapComponent>();

	for (auto [entity, transform, cubemap] : cubemapView.each()) {
		RendererCubemap::the().drawCubemap(transform, cubemap.color, cubemap.texture);
	}
}

} // namespace Inferno
