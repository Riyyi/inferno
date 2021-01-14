#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string> // std::string

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm::vec3

#include "inferno/log.h"

namespace Inferno {

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator const std::string&() const { return tag; }
	};

	struct TransformComponent {
		glm::vec3 translate { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotate { 0.0f, 0.0f, 0.0f } ;
		glm::vec3 scale { 1.0f, 1.0f, 1.0f };
		glm::mat4 transform { 1.0f }; // Identity matrix
	};

	struct OrthographicCameraComponment {
		float zoomLevel = 1.0f;
		glm::vec3 rotateAxis { 0.0f, 0.0f, 1.0f };
		glm::mat4 projection { 1.0f }; // Identity matrix
	};

	struct PerspectiveCameraComponent {
		float fov = 90.0f;
		float pitch = 0.0f;
		float yaw = -90.0f;
		glm::vec3 up { 0.0f, 1.0f, 0.0f };
		glm::mat4 projection { 1.0f }; // Identity matrix
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const TransformComponent& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec4& value);
	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value);

}

#endif // COMPONENTS_H
