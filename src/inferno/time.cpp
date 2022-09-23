/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "GLFW/glfw3.h"

#include "inferno/time.h"

namespace Inferno {

float Time::time()
{
	return static_cast<float>(glfwGetTime());
}

} // namespace Inferno
