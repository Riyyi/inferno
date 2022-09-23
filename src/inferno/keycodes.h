/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

#define MAP_KEY(key) #key, key

namespace Inferno {

int keyCode(std::string_view name);
std::string_view keyName(int);

} // namespace Inferno
