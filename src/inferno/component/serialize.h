/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "ruc/json/json.h"

namespace glm {

void toJson(ruc::Json& json, const vec2& value);
void fromJson(const ruc::Json& json, vec2& value);

void toJson(ruc::Json& json, const vec3& value);
void fromJson(const ruc::Json& json, vec3& value);

void toJson(ruc::Json& json, const vec4& value);
void fromJson(const ruc::Json& json, vec4& value);

} // namespace glm

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
struct ruc::format::Formatter<glm::mat2> : Formatter<glm::vec2> {
	void format(Builder& builder, glm::mat2 value) const;
};

template<>
struct ruc::format::Formatter<glm::mat3> : Formatter<glm::vec3> {
	void format(Builder& builder, glm::mat3 value) const;
};

template<>
struct ruc::format::Formatter<glm::mat4> : Formatter<glm::vec4> {
	void format(Builder& builder, glm::mat4 value) const;
};
