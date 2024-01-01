/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/id-component.h"

namespace Inferno {

void fromJson(const ruc::Json& json, IDComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("id")) {
		json.at("id").getTo(value.id);
	}
}

} // namespace Inferno
