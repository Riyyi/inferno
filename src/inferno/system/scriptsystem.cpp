#include "entt/entity/registry.hpp" // entt::entity, entt::registry

#include "inferno/component/luascriptcomponent.h"
#include "inferno/component/nativescriptcomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/io/log.h"
#include "inferno/scene/scene.h"
#include "inferno/script/luascript.h"
#include "inferno/script/nativescript.h"
#include "inferno/system/scriptsystem.h"

namespace Inferno {

ScriptSystem::ScriptSystem(s)
{
	info() << "ScriptSystem initialized";
}

ScriptSystem::~ScriptSystem()
{
	auto nativeScriptView = m_scene->registry()->view<NativeScriptComponent>();

	for (auto entity : nativeScriptView) {
		cleanup(nativeScriptView.get<NativeScriptComponent>(entity));
	}

	auto luaScriptView = m_scene->registry()->view<LuaScriptComponent>();

	for (auto entity : luaScriptView) {
		cleanup(luaScriptView.get<LuaScriptComponent>(entity));
	}
}

void ScriptSystem::update(float deltaTime)
{
	// @Todo figure out why group doesn't work here
	auto nativeScriptView = m_scene->registry()->view<TransformComponent, NativeScriptComponent>();

	for (auto [entity, transform, nativeScript] : nativeScriptView.each()) {

		// Create native script if not initialized
		if (!nativeScript.instance) {
			nativeScript.instance = nativeScript.initialize();
			nativeScript.instance->transform = &transform;
			nativeScript.instance->m_scene = m_scene;
			nativeScript.instance->m_entity = static_cast<uint32_t>(entity);
			nativeScript.instance->initialize();
		}

		nativeScript.instance->transform = &transform;
		nativeScript.instance->update(deltaTime);
	}

	auto luaScriptView = m_scene->registry()->view<TransformComponent, LuaScriptComponent>();

	for (auto [entity, transform, luaScript] : luaScriptView.each()) {

		// Create Lua script if not initialized
		if (!luaScript.instance) {
			luaScript.instance = new LuaScript();
			luaScript.instance->transform = &transform;
			luaScript.instance->m_scene = m_scene;
			luaScript.instance->m_entity = static_cast<uint32_t>(entity);
			luaScript.instance->m_path = luaScript.path;
			luaScript.instance->initialize();
		}

		luaScript.instance->update(deltaTime);
	}
}

void ScriptSystem::cleanup(uint32_t entity)
{
	if (m_scene->hasComponent<NativeScriptComponent>(entity)) {
		auto& nativeScript = m_scene->getComponent<NativeScriptComponent>(entity);
		cleanup(nativeScript);
	}

	if (m_scene->hasComponent<LuaScriptComponent>(entity)) {
		auto& luaScript = m_scene->getComponent<LuaScriptComponent>(entity);
		cleanup(luaScript);
	}
}

void ScriptSystem::cleanup(NativeScriptComponent& nativeScript)
{
	if (nativeScript.instance) {
		nativeScript.instance->destroy();
		nativeScript.destroy();
	}
}

void ScriptSystem::cleanup(LuaScriptComponent& luaScript)
{
	if (luaScript.instance) {
		luaScript.instance->destroy();
		delete luaScript.instance;
	}
}

} // namespace Inferno
