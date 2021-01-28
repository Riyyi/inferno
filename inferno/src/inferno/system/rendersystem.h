#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry

#include "inferno/render/renderer.h"

namespace Inferno {

	class RenderSystem {
	public:
		void initialize();
		void render();
		void destroy();

		void setRegistry(const std::shared_ptr<entt::registry>& registry) { m_registry = registry; };

		static inline RenderSystem& the() { return *s_instance; }

	private:
		std::shared_ptr<entt::registry> m_registry;

		static RenderSystem* s_instance;
	};

}

#endif // RENDER_SYSTEM_H
