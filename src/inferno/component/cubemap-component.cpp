/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/cubemap-component.h"
#include "inferno/component/spritecomponent.h" // TODO: Move glm::x toJson/fromJson to separate file
#include "inferno/render/texture.h"

namespace Inferno {

void fromJson(const ruc::Json& json, CubemapComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("color")) {
		json.at("color").getTo(value.color);
	}
	if (json.exists("texture") && json.at("texture").type() == ruc::Json::Type::String) {
		value.texture = TextureManager::the().load(json.at("texture").asString(), Texture::Type::Cubemap);
	}
}

} // namespace Inferno
