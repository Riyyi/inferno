#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory> //std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry
#include "ruc/singleton.h"

#include "inferno/render/renderer.h"

namespace Inferno {

	class RenderSystem final : public ruc::Singleton<RenderSystem> {
	public:
		RenderSystem(s);
		virtual ~RenderSystem();

		void render();

		void setRegistry(std::shared_ptr<entt::registry> registry) { m_registry = registry; };

	private:
		std::shared_ptr<entt::registry> m_registry;
	};

} // namespace Inferno

#endif // RENDER_SYSTEM_H
