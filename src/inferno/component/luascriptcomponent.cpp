/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/luascriptcomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, LuaScriptComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	VERIFY(json.exists("path"), "path not found");
	json.at("path").getTo(value.path);
}

} // namespace Inferno
