/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "ruc/json/json.h"

#include "inferno/component/serialize.h"

namespace glm {

void toJson(ruc::Json& json, const vec2& value)
{
	json = ruc::Json {
		{ value.x, value.y },
	};
}

void fromJson(const ruc::Json& json, vec2& value)
{
	VERIFY(json.type() == ruc::Json::Type::Array);

	auto& values = json.asArray();
	VERIFY(values.size() == 2, "glm::vec2 expected 2 values, got: {}", values.size());

	value.x = values.at(0).get<float>();
	value.y = values.at(1).get<float>();
}

// -----------------------------------------

void toJson(ruc::Json& json, const vec3& value)
{
	json = ruc::Json {
		{ value.x, value.y, value.z },
	};
}

void fromJson(const ruc::Json& json, vec3& value)
{
	VERIFY(json.type() == ruc::Json::Type::Array);

	auto& values = json.asArray();
	VERIFY(values.size() == 3, "glm::vec3 expected 3 values, got: {}", values.size());

	value.x = values.at(0).get<float>();
	value.y = values.at(1).get<float>();
	value.z = values.at(2).get<float>();
}

// -----------------------------------------

void toJson(ruc::Json& json, const vec4& value)
{
	json = ruc::Json {
		{ value.r, value.g, value.b, value.a },
	};
}

void fromJson(const ruc::Json& json, vec4& value)
{
	VERIFY(json.type() == ruc::Json::Type::Array);

	auto& values = json.asArray();
	VERIFY(values.size() == 4, "glm::vec4 expected 4 values, got: {}", values.size());

	value.r = values.at(0).get<float>();
	value.g = values.at(1).get<float>();
	value.b = values.at(2).get<float>();
	value.a = values.at(3).get<float>();
}

} // namespace glm

// -----------------------------------------

void ruc::format::Formatter<glm::vec2>::format(Builder& builder, glm::vec2 value) const
{
	return Formatter<std::vector<float>>::format(builder, { value.x, value.y });
}

void ruc::format::Formatter<glm::vec3>::format(Builder& builder, glm::vec3 value) const
{
	return Formatter<std::vector<float>>::format(builder, { value.x, value.y, value.z });
}

void ruc::format::Formatter<glm::vec4>::format(Builder& builder, glm::vec4 value) const
{
	return Formatter<std::vector<float>>::format(builder, { value.x, value.y, value.z, value.w });
}

void ruc::format::Formatter<glm::mat2>::format(Builder& builder, glm::mat2 value) const
{
	builder.putString("mat2 ");
	Formatter<glm::vec2>::format(builder, value[0]);
	builder.putString("\n     ");
	return Formatter<glm::vec2>::format(builder, value[1]);
}

void ruc::format::Formatter<glm::mat3>::format(Builder& builder, glm::mat3 value) const
{
	builder.putString("mat3 ");
	Formatter<glm::vec3>::format(builder, value[0]);
	builder.putString("\n     ");
	Formatter<glm::vec3>::format(builder, value[1]);
	builder.putString("\n     ");
	return Formatter<glm::vec3>::format(builder, value[2]);
}

void ruc::format::Formatter<glm::mat4>::format(Builder& builder, glm::mat4 value) const
{
	builder.putString("mat4 ");
	Formatter<glm::vec4>::format(builder, value[0]);
	builder.putString("\n     ");
	Formatter<glm::vec4>::format(builder, value[1]);
	builder.putString("\n     ");
	Formatter<glm::vec4>::format(builder, value[2]);
	builder.putString("\n     ");
	return Formatter<glm::vec4>::format(builder, value[3]);
}
