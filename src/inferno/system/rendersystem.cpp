/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::radians
#include "ruc/format/log.h"

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
	auto group = m_registry->group<TransformComponent, SpriteComponent>();

	for (auto [entity, transform, sprite] : group.each()) {
		Renderer2D::the().drawQuad(transform, sprite.color, sprite.texture);
	}
}

} // namespace Inferno
