#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::radians

#include "inferno/assert.h"
#include "inferno/components/spritecomponent.h"
#include "inferno/components/transformcomponent.h"
#include "inferno/io/log.h"
#include "inferno/render/renderer.h"
#include "inferno/systems/rendersystem.h"

namespace Inferno {

	RenderSystem* RenderSystem::s_instance = nullptr;

	void RenderSystem::initialize()
	{
		ASSERT(!s_instance, "RenderSystem already exists!");
		s_instance = this;

		dbg(Log::Info) << "RenderSystem initialized";
	}

	void RenderSystem::render()
	{
		auto group = m_registry->group<TransformComponent, SpriteComponent>();

		for (auto [entity, transform, sprite] : group.each()) {
			Renderer2D::the().drawQuad(transform, sprite.color, sprite.texture);
		}
	}

	void RenderSystem::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

}
