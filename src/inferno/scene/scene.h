/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef> // size_t
#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr

#include "entt/entity/registry.hpp"    // entt::entity, entt::registry
#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "ruc/format/format.h"
#include "ruc/json/json.h"

#include "inferno/uid.h"

namespace Inferno {

class Camera;
class Texture;

class Scene {
public:
	void initialize();
	void update(float deltaTime);
	void render();
	void destroy();

	uint32_t createEntity(const std::string& name = "");
	uint32_t createEntityWithUID(UID id, const std::string& name = "");
	uint32_t findEntity(std::string_view name);
	void destroyEntity(uint32_t entity);

	glm::mat4 cameraProjectionView();

	void validEntity(uint32_t entity) const;

	template<typename... T>
	[[nodiscard]] bool hasComponent(uint32_t entity) const
	{
		validEntity(entity);
		return m_registry->has<T...>(entt::entity { entity });
	}

	template<typename... T>
	[[nodiscard]] bool anyComponent(uint32_t entity) const
	{
		validEntity(entity);
		return m_registry->any<T...>(entt::entity { entity });
	}

	// TODO: Should replace be allowed? could trigger memory leaks with nativescript
	// TODO: Replace will make it so an entity cant have multiple scripts
	template<typename T, typename... P>
	T& addComponent(uint32_t entity, P&&... parameters) const
	{
		validEntity(entity);
		return m_registry->emplace_or_replace<T>(entt::entity { entity }, std::forward<P>(parameters)...);
	}

	template<typename T>
	size_t removeComponent(uint32_t entity) const
	{
		validEntity(entity);
		return m_registry->remove_if_exists<T>(entt::entity { entity });
	}

	// TODO: Should replace be allowed? could trigger memory leaks with nativescript
	template<typename T, typename... P>
	T& getComponent(uint32_t entity, P&&... parameters) const
	{
		validEntity(entity);
		return m_registry->get_or_emplace<T>(entt::entity { entity }, std::forward<P>(parameters)...);
	}

	// const entt::registry& registry() const { return m_registry; }
	std::shared_ptr<entt::registry> registry() const { return m_registry; }

private:
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<Texture> m_texture2;

	std::shared_ptr<entt::registry> m_registry;
};

} // namespace Inferno

template<>
struct ruc::format::Formatter<entt::entity> : Formatter<uint32_t> {
	void format(Builder& builder, entt::entity value) const;
};

// @Todo
// - Convert registry to stack variable
// - Convert registry to scene pointer in systems
