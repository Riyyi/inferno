/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "inferno/component/spritecomponent.h"
#include "inferno/render/texture.h"

namespace Inferno {

void fromJson(const ruc::Json& json, SpriteComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("color")) {
		json.at("color").getTo(value.color);
	}
	if (json.exists("texture") && json.at("texture").type() == ruc::Json::Type::String) {
		value.texture = TextureManager::the().load(json.at("texture").asString());
	}
}

} // namespace Inferno

namespace glm {

void toJson(ruc::Json& json, const vec4& value)
{
	json = ruc::Json {
		{ value.r, value.g, value.b, value.a },
	};
}

void fromJson(const ruc::Json& json, vec4& value)
{
	VERIFY(json.type() == ruc::Json::Type::Array);

	auto& values = json.asArray();
	VERIFY(values.size() == 4, "glm::vec4 expects 4 values, not {}", values.size());

	value.r = values.at(0).get<float>();
	value.g = values.at(1).get<float>();
	value.b = values.at(2).get<float>();
	value.a = values.at(3).get<float>();
}

} // namespace glm
