#ifndef SCENE_H
#define SCENE_H

#include <cstdint> // uint32_t
#include <memory> // std::shared_ptr

#include "entt/entity/registry.hpp"    // entt::entity, entt::registry

namespace Inferno {

	class Entity;

	class Scene {
	public:
		Scene();
		virtual ~Scene() = default;

		Entity createEntity(const std::string& name = "");
		Entity createEntity(entt::entity handle);
		Entity createEntity(uint32_t handle);

	private:
		std::shared_ptr<entt::registry> m_registry;
	};

}

#endif // SCENE_H
