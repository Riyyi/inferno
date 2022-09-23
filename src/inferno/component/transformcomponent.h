/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm::vec3
#include "ruc/format/format.h"

namespace Inferno {

struct TransformComponent {
	glm::vec3 translate { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotate { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale { 1.0f, 1.0f, 1.0f };
	glm::mat4 transform { 1.0f }; // Identity matrix
};

} // namespace Inferno

template<>
struct ruc::format::Formatter<glm::vec2> : Formatter<std::vector<float>> {
	void format(Builder& builder, glm::vec2 value) const;
};

template<>
struct ruc::format::Formatter<glm::vec3> : Formatter<std::vector<float>> {
	void format(Builder& builder, glm::vec3 value) const;
};

template<>
struct ruc::format::Formatter<glm::vec4> : Formatter<std::vector<float>> {
	void format(Builder& builder, glm::vec4 value) const;
};

template<>
struct ruc::format::Formatter<glm::mat4> : Formatter<glm::vec4> {
	void format(Builder& builder, glm::mat4 value) const;
};

template<>
struct ruc::format::Formatter<Inferno::TransformComponent> : Formatter<glm::vec3> {
	void format(Builder& builder, Inferno::TransformComponent value) const;
};
