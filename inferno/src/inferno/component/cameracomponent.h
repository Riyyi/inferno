#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm::vec3

namespace Inferno {

	enum CameraType {
		Orthographic,
		Perspective,
	};

	struct CameraComponent {
		CameraType type = CameraType::Perspective;

		// Orthographic
		float zoomLevel = 1.0f;
		glm::vec3 rotateAxis { 0.0f, 0.0f, 1.0f };

		// Perspective
		float fov = 90.0f;
		float pitch = 0.0f;
		float yaw = -90.0f;
		glm::vec3 up { 0.0f, 1.0f, 0.0f };

		glm::mat4 projection { 1.0f }; // Identity matrix
	};

}

#endif // CAMERA_COMPONENT_H
