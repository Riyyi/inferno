/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/nativescriptcomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, NativeScriptComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	VERIFY(json.exists("name"), "name not found");
	json.at("name").getTo(value.name);
}

} // namespace Inferno
