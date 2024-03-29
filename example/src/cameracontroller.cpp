/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glm/ext/matrix_transform.hpp" // glm::radians

#include "inferno/component/cameracomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/io/input.h"
#include "inferno/keycodes.h"

#include "cameracontroller.h"

namespace example {

void CameraController::updateOrthographic(float deltaTime)
{
	// Update camera rotation

	float cameraRotateSpeed = ROTATE_SPEED * deltaTime;

	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_Q"))) {
		transform->rotate.z -= cameraRotateSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_E"))) {
		transform->rotate.z += cameraRotateSpeed;
	}

	if (transform->rotate.z > 180.0f) {
		transform->rotate.z -= 360.0f;
	}
	else if (transform->rotate.z <= -180.0f) {
		transform->rotate.z += 360.0f;
	}

	// Update camera translation

	float cameraTranslateSpeed = TRANSLATE_SPEED * deltaTime;

	// WASD movement
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_W"))) {
		transform->translate.x += -sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
		transform->translate.y += cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_S"))) {
		transform->translate.x -= -sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
		transform->translate.y -= cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_A"))) {
		transform->translate.x -= cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
		transform->translate.y -= sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_D"))) {
		transform->translate.x += cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
		transform->translate.y += sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
	}

	// Update camera zoom

	float zoomSpeed = ZOOM_SENSITIVITY * deltaTime;

	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_EQUAL"))) {
		m_camera->zoomLevel -= zoomSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_MINUS"))) {
		m_camera->zoomLevel += zoomSpeed;
	}
	m_camera->zoomLevel = std::max(m_camera->zoomLevel, 0.25f);
	m_camera->zoomLevel = std::min(m_camera->zoomLevel, 10.0f);
}

void CameraController::updatePerspective(float deltaTime)
{
	// Get mouse movement offset compared to last frame
	float xOffset = Inferno::Input::getXOffset() * MOUSE_SENSITIVITY;
	float yOffset = Inferno::Input::getYOffset() * MOUSE_SENSITIVITY;
	m_camera->yaw += xOffset;
	m_camera->pitch += yOffset;

	// Prevent gimbal lock
	if (m_camera->pitch > 89.0f)
		m_camera->pitch = 89.0f;
	if (m_camera->pitch < -89.0f)
		m_camera->pitch = -89.0f;

	// Update camera rotation, by calculating direction vector via yaw and pitch

	transform->rotate = {
		cos(glm::radians(m_camera->pitch)) * cos(glm::radians(m_camera->yaw)),
		sin(glm::radians(m_camera->pitch)),
		cos(glm::radians(m_camera->pitch)) * sin(glm::radians(m_camera->yaw))
	};
	transform->rotate = glm::normalize(transform->rotate);
	// The direction vector is based on
	// Camera direction (z): normalize(position - target)
	// Right axis       (x): normalize(cross(up, direction))
	// Up axis          (y): cross(direction, right)

	// Source: https://learnopengl.com/img/getting-started/camera_axes.png

	// Cross = combination of two vectors in 3D space,
	//         where result is always perpendicular to both of the vectors

	// Update camera translation

	float cameraSpeed = TRANSLATE_SPEED * deltaTime;

	// WASD movement
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_W"))) {
		transform->translate += transform->rotate * cameraSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_S"))) {
		transform->translate -= transform->rotate * cameraSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_A"))) {
		transform->translate -= glm::normalize(glm::cross(transform->rotate, m_camera->up)) * cameraSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_D"))) {
		transform->translate += glm::normalize(glm::cross(transform->rotate, m_camera->up)) * cameraSpeed;
	}
	// Up / down movement
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_SPACE"))) {
		transform->translate.y += cameraSpeed;
	}
	if (Inferno::Input::isKeyPressed(Inferno::keyCode("GLFW_KEY_LEFT_SHIFT"))) {
		transform->translate.y -= cameraSpeed;
	}
}

} // namespace example
