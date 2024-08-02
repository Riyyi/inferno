/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::shared_ptr

#include "ruc/json/json.h"

#include "inferno/asset/model.h"
#include "inferno/asset/texture.h"

namespace Inferno {

struct ModelComponent {
	std::shared_ptr<Model> model;
	std::shared_ptr<Texture2D> texture;
};

void fromJson(const ruc::Json& json, ModelComponent& value);

} // namespace Inferno
