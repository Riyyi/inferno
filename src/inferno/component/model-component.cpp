/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "inferno/component/model-component.h"
#include "inferno/asset/asset-manager.h"
#include "inferno/asset/model.h"
#include "inferno/asset/texture.h"
#include "inferno/component/serialize.h" // not detected as used by clang-tidy

namespace Inferno {

void fromJson(const ruc::Json& json, ModelComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("color")) {
		json.at("color").getTo(value.color);
	}
	if (json.exists("model") && json.at("model").type() == ruc::Json::Type::String) {
		value.model = AssetManager::the().load<Model>(json.at("model").asString());
	}
	if (json.exists("texture") && json.at("texture").type() == ruc::Json::Type::String) {
		value.texture = AssetManager::the().load<Texture2D>(json.at("texture").asString());
	}
}

} // namespace Inferno
