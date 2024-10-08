/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>  //std::shared_ptr
#include <utility> // std::pair

#include "entt/entity/registry.hpp"    // entt::entity, entt::registry
#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "ruc/singleton.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

namespace Inferno {

struct TransformComponent;
struct CameraComponent;

class CameraSystem final : public ruc::Singleton<CameraSystem> {
public:
	CameraSystem(s);
	virtual ~CameraSystem();

	void update();

	/**
	 * @brief Return a pair from the camera component: { projection, view }
	 */
	std::pair<glm::mat4, glm::mat4> projectionView();
	glm::vec3 translate();

	void setRegistry(std::shared_ptr<entt::registry> registry) { m_registry = registry; };

private:
	void updateOrthographic(TransformComponent& transform, CameraComponent& camera);
	void updatePerspective(TransformComponent& transform, CameraComponent& camera);

	std::shared_ptr<entt::registry> m_registry;
};

} // namespace Inferno
