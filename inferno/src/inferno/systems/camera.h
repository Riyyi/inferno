#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#define TRANSLATE_SPEED 2.5f
#define ROTATE_SPEED 90.0f
#define ZOOM_SENSITIVITY 2.5f
#define MOUSE_SENSITIVITY 0.25f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry

#include "inferno/scene/components.h"

namespace Inferno {

	class Entity;

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
