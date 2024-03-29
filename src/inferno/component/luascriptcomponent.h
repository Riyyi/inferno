/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <utility> // std::move

#include "ruc/json/json.h"

namespace Inferno {

class LuaScript;

struct LuaScriptComponent {
	LuaScript* instance { nullptr };
	std::string path;

	// Dont allow manually setting instance during construction
	LuaScriptComponent() {}
	LuaScriptComponent(const std::string& path)
		: path(std::move(path))
	{
	}
};

void fromJson(const ruc::Json& json, LuaScriptComponent& value);

} // namespace Inferno
