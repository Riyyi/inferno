#ifndef SCENE_H
#define SCENE_H

#include <cstdint> // uint32_t
#include <memory> // std::shared_ptr

#include "entt/entity/registry.hpp"    // entt::entity, entt::registry
#include "glm/ext/matrix_float4x4.hpp" // glm::mat4

namespace Inferno {

	class Camera;
	class Entity;

	class Scene {
	public:
		Scene();
		virtual ~Scene();

		void initialize();
		void update(float deltaTime);
		void render();
		void destroy();

		Entity createEntity(const std::string& name = "");
		Entity createEntity(entt::entity handle);
		Entity createEntity(uint32_t handle);

		glm::mat4 cameraProjectionView();

	private:
		std::shared_ptr<entt::registry> m_registry;
	};

}

#endif // SCENE_H
