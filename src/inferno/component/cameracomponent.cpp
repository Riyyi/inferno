/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/format/print.h"
#include "ruc/json/json.h"

#include "inferno/component/cameracomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, CameraComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("type")) {
		value.type = json.at("type").get<std::string>() == "orthographic" ? CameraType::Orthographic : CameraType::Perspective;
	}
	if (json.exists("zoom-level")) {
		json.at("zoom-level").getTo(value.zoomLevel);
	}
}

} // namespace Inferno
