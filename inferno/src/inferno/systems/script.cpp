#include "entt/entity/registry.hpp" // entt::entity, entt::registry

#include "inferno/assertions.h"
#include "inferno/scene/components.h"
#include "inferno/script/nativescript.h"
#include "inferno/systems/script.h"
#include <bits/stdint-uintn.h>

namespace Inferno {

	ScriptSystem* ScriptSystem::s_instance = nullptr;

	void ScriptSystem::initialize()
	{
		ASSERT(!s_instance, "ScriptSystem already exists!");
		s_instance = this;

		dbg(Log::Info) << "ScriptSystem initialized";
	}

	void ScriptSystem::destroy()
	{
		auto view = m_scene->registry()->view<NativeScriptComponent>();

		// @Todo change the getComponent() call to retrieve from the view for more performance
		for (auto entity : view) {
			cleanup(static_cast<uint32_t>(entity));
		}

		delete s_instance;
		s_instance = nullptr;
	}

	void ScriptSystem::update(float deltaTime)
	{
		// @Todo figure out why group doesn't work here
		auto view = m_scene->registry()->view<TransformComponent, NativeScriptComponent>();

		for (auto [entity, transform, nativeScript] : view.each()) {

			// Create script if not initialized
			if (!nativeScript.instance) {
				nativeScript.initialize();
				nativeScript.instance->transform = &transform;
				nativeScript.instance->m_scene = m_scene;
				nativeScript.instance->m_entity = static_cast<uint32_t>(entity);
				nativeScript.instance->initialize();
			}

			nativeScript.instance->transform = &transform;
			nativeScript.instance->update(deltaTime);
		}
	}

	void ScriptSystem::cleanup(uint32_t entity)
	{
		auto& nativeScript = m_scene->getComponent<NativeScriptComponent>(entity);

		if (!nativeScript.instance) {
			nativeScript.instance->destroy();
			nativeScript.destroy();
		}
	}

}
