#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry
#include "inferno/singleton.h"

namespace Inferno {

	class TransformSystem final : public Singleton<TransformSystem> {
	public:
		TransformSystem(s);
		virtual ~TransformSystem();

		void update();

		void setRegistry(const std::shared_ptr<entt::registry>& registry) { m_registry = registry; };

	private:
		std::shared_ptr<entt::registry> m_registry;
	};

}

#endif // TRANSFORM_SYSTEM_H
