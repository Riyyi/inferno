/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::shared_ptr

#include "glm/ext/vector_float4.hpp" // glm::vec4
#include "ruc/json/json.h"

#include "inferno/render/texture.h"

namespace Inferno {

struct SpriteComponent {
	glm::vec4 color { 1.0f };
	std::shared_ptr<Texture> texture;
};

void fromJson(const ruc::Json& json, SpriteComponent& value);

} // namespace Inferno

namespace glm {

void toJson(ruc::Json& json, const vec4& value);
void fromJson(const ruc::Json& json, vec4& value);

} // namespace glm
