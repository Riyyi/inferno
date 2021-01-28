#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm::vec3

#include "inferno/io/log.h"

namespace Inferno {

	struct TransformComponent {
		glm::vec3 translate { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotate { 0.0f, 0.0f, 0.0f } ;
		glm::vec3 scale { 1.0f, 1.0f, 1.0f };
		glm::mat4 transform { 1.0f }; // Identity matrix
	};

// -----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const glm::vec2& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec4& value);
	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value);
	const LogStream& operator<<(const LogStream& stream, const TransformComponent& value);
}

#endif // TRANSFORM_COMPONENT_H
