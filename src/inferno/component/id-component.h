/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "ruc/json/json.h"

#include "inferno/uid.h"

namespace Inferno {

struct IDComponent {
	UID id;

	IDComponent() = default;
	IDComponent(UID id)
		: id(id)
	{
	}
	IDComponent(const IDComponent&) = default;
};

void fromJson(const ruc::Json& json, IDComponent& value);

} // namespace Inferno
