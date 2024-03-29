/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/file.h"
#include "sol/unsafe_function_result.hpp"

#include "inferno/component/cameracomponent.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/tagcomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/scene/scene.h"
#include "inferno/script/luascript.h"
#include "inferno/script/registration.h"

namespace Inferno {

void LuaScript::initialize()
{
	// Type registration
	// ---------------------------------

	Registration::fill(m_state);

	// Load libraries
	// ---------------------------------

	m_state.open_libraries(sol::lib::base, sol::lib::math);

	// Component get functions
	// ---------------------------------

	m_state.set_function("getTagComponent", [this]() -> TagComponent* {
		return &m_scene->getComponent<TagComponent>(m_entity);
	});

	m_state.set_function("getTransformComponent", [this]() -> TransformComponent* {
		return &m_scene->getComponent<TransformComponent>(m_entity);
	});

	m_state.set_function("getCameraComponent", [this]() -> CameraComponent* {
		return &m_scene->getComponent<CameraComponent>(m_entity);
	});

	m_state.set_function("getSpriteComponent", [this]() -> SpriteComponent* {
		return &m_scene->getComponent<SpriteComponent>(m_entity);
	});

	// Load and Initialize script
	// ---------------------------------

	loadScript();

	callFunction("LuaScript", "initialize");
}

void LuaScript::destroy()
{
	callFunction("LuaScript", "destroy");
}

void LuaScript::update(float deltaTime)
{
	m_state["LuaScript"]["transform"] = &m_scene->getComponent<TransformComponent>(m_entity);
	callFunction("LuaScript", "update", deltaTime);
}

void LuaScript::loadScript()
{
	std::string script = ruc::File(m_path).data();
	auto result = m_state.script(script.c_str(),
	                             [](lua_State*, sol::protected_function_result pfr) { return pfr; });
	VERIFY(result.valid(), "LuaScript {}", ((sol::error)result).what());
}

sol::table LuaScript::getTable(const char* name)
{
	sol::table table = m_state[name];
	VERIFY(table.valid(), "LuaScript table does not exist");
	return table;
}

} // namespace Inferno
