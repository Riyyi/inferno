/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/asset/asset-manager.h"
#include "inferno/asset/texture.h"
#include "inferno/component/serialize.h" // not detected as used by clang-tidy
#include "inferno/component/spritecomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, SpriteComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("color")) {
		json.at("color").getTo(value.color);
	}
	if (json.exists("texture") && json.at("texture").type() == ruc::Json::Type::String) {
		value.texture = AssetManager::the().load<Texture2D>(json.at("texture").asString());
	}
}

} // namespace Inferno
