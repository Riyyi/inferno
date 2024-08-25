/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/format/format.h"
#include "ruc/json/json.h"

#include "inferno/component/serialize.h"
#include "inferno/component/transformcomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, TransformComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("translate")) {
		json.at("translate").getTo(value.translate);
	}
	if (json.exists("rotate")) {
		json.at("rotate").getTo(value.rotate);
	}
	if (json.exists("scale")) {
		json.at("scale").getTo(value.scale);
	}
}

} // namespace Inferno

void ruc::format::Formatter<Inferno::TransformComponent>::format(Builder& builder, Inferno::TransformComponent value) const
{
	builder.putString("transform ");
	Formatter<glm::vec3>::format(builder, value.translate);
	builder.putString(" t\n          ");
	Formatter<glm::vec3>::format(builder, value.rotate);
	builder.putString(" r\n          ");
	Formatter<glm::vec3>::format(builder, value.scale);
	builder.putString(" s");
}
