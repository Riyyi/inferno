/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::shared_ptr

#include "glm/ext/vector_float4.hpp" // glm::vec4
#include "ruc/json/json.h"

#include "inferno/asset/texture.h"

namespace Inferno {

struct CubemapComponent {
	glm::vec4 color { 1.0f };
	std::shared_ptr<Texture> texture;
	bool isLight { false };
};

void fromJson(const ruc::Json& json, CubemapComponent& value);

} // namespace Inferno
