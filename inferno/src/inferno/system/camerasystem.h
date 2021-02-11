#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry

#include "inferno/component/cameracomponent.h"
#include "inferno/component/transformcomponent.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

namespace Inferno {

	class CameraSystem {
	public:
		void initialize();
		void update();
		void destroy();

		glm::mat4 projectionView();

		void setRegistry(const std::shared_ptr<entt::registry>& registry) { m_registry = registry; };

		static inline CameraSystem& the() { return *s_instance; }

	private:
		void updateOrthographic(TransformComponent& transform, CameraComponent& camera);
		void updatePerspective(TransformComponent& transform, CameraComponent& camera);

		std::shared_ptr<entt::registry> m_registry;

		static CameraSystem* s_instance;
	};

}

#endif // CAMERA_SYSTEM_H
