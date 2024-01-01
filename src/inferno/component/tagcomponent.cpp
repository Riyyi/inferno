/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/tagcomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, TagComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("tag")) {
		json.at("tag").getTo(value.tag);
	}
}

} // namespace Inferno
