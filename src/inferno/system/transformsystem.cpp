/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // std::sort
#include <cstdint>   // uint32_t

#include "entt/entity/fwd.hpp"          // entt:entity
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::radians
#include "ruc/format/log.h"

#include "inferno/component/transformcomponent.h"
#include "inferno/system/transformsystem.h"

namespace Inferno {

TransformSystem::TransformSystem(s)
{
	ruc::info("TransformSystem initialized");
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::update()
{
	auto view = m_registry->view<TransformComponent>();

	for (auto entity : m_hierarchy) {

		auto& component = view.get<TransformComponent>(entity);

		// Identity matrix
		component.transform = glm::mat4(1.0f);

		// Translate
		component.transform = glm::translate(component.transform, component.translate);

		// Rotate
		component.transform = glm::rotate(component.transform, glm::radians(component.rotate.x), { 1.0, 0.0, 0.0 });
		component.transform = glm::rotate(component.transform, glm::radians(component.rotate.y), { 0.0, 1.0, 0.0 });
		component.transform = glm::rotate(component.transform, glm::radians(component.rotate.z), { 0.0, 0.0, 1.0 });

		// Scale
		component.transform = glm::scale(component.transform, component.scale);

		// Apply the parent transform to the child transform
		if (component.parent != entt::null) {
			auto& parent = view.get<TransformComponent>(component.parent);
			component.transform = parent.transform * component.transform;
		}
	}
}

void TransformSystem::add(uint32_t entity)
{
	m_hierarchy.push_back(static_cast<entt::entity>(entity));
}

void TransformSystem::sort()
{
	std::sort(m_hierarchy.begin(), m_hierarchy.end(),
	          [](entt::entity a, entt::entity b) {
				  return (a == entt::null && b == entt::null) ? false
		                 : (a == entt::null)                  ? true
		                 : (b == entt::null)                  ? false
		                                                      : a < b;
			  });
}

} // namespace Inferno
