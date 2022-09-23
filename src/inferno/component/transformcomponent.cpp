/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "inferno/component/transformcomponent.h"

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

void ruc::format::Formatter<Inferno::TransformComponent>::format(Builder& builder, Inferno::TransformComponent value) const
{
	builder.putString("transform ");
	Formatter<glm::vec3>::format(builder, value.translate);
	builder.putString(" t\n          ");
	Formatter<glm::vec3>::format(builder, value.rotate);
	builder.putString(" r\n          ");
	Formatter<glm::vec3>::format(builder, value.scale);
	builder.putString(" s");
}
