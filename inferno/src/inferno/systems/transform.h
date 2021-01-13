#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry

namespace Inferno {

	class TransformSystem {
	public:
		void initialize();
		void update();
		void destroy();

		void setRegistry(const std::shared_ptr<entt::registry>& registry) { m_registry = registry; };

		static inline TransformSystem& the() { return *s_instance; }

	private:
		std::shared_ptr<entt::registry> m_registry;

		static TransformSystem* s_instance;
	};

}

#endif // TRANSFORM_SYSTEM_H
