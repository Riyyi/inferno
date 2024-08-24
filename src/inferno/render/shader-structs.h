/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "glm/ext/vector_float3.hpp" // glm::vec3

namespace Inferno {

// Shader storage block layouts, using std430 memory layout rules

#define MAX_DIRECTIONAL_LIGHTS 4
struct alignas(16) DirectionalLightBlock {
	alignas(16) glm::vec3 direction { 0 };

	alignas(16) glm::vec3 ambient { 0 };
	alignas(16) glm::vec3 diffuse { 0 };
	alignas(16) glm::vec3 specular { 0 };
};

} // namespace Inferno
