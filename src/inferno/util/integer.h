/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <limits>  // std::numeric_limits
#include <string>  // std::stoul
#include <string_view>

#include "ruc/meta/assert.h"

namespace std {

// Can't believe this is not in the standard library

inline uint32_t stou(string_view string)
{
	unsigned long size = std::stoul(string.data());
	VERIFY(size <= std::numeric_limits<uint32_t>::max(), "string not in uint32_t range '{}'", string);
	return static_cast<uint32_t>(size);
}

} // namespace std
