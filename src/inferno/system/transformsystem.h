/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr
#include <vector>

#include "entt/entity/registry.hpp" // entt::entity, entt::registry
#include "ruc/singleton.h"

namespace Inferno {

class TransformSystem final : public ruc::Singleton<TransformSystem> {
public:
	TransformSystem(s);
	virtual ~TransformSystem();

	void update();

	void add(uint32_t entity);
	void sort();

	void setRegistry(std::shared_ptr<entt::registry> registry) { m_registry = registry; };

private:
	std::vector<entt::entity> m_hierarchy;

	std::shared_ptr<entt::registry> m_registry;
};

} // namespace Inferno
