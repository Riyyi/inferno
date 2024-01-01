/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>  // std::string
#include <utility> // std::move

#include "ruc/json/json.h"

namespace Inferno {

struct TagComponent {
	std::string tag;

	TagComponent() = default;
	TagComponent(const std::string& tag)
		: tag(std::move(tag))
	{
	}

	operator const std::string&() const { return tag; }
};

void fromJson(const ruc::Json& json, TagComponent& value);

} // namespace Inferno
