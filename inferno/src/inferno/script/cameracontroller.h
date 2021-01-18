#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define TRANSLATE_SPEED 2.5f
#define ROTATE_SPEED 90.0f
#define ZOOM_SENSITIVITY 2.5f
#define MOUSE_SENSITIVITY 0.25f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective, glm::ortho
#include <glm/ext/matrix_transform.hpp>  // glm::radians, glm::lookAt

#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/scene/components.h"
#include "inferno/script/nativescript.h"

namespace Inferno {

	class CameraController final : public NativeScript {
	public:
		CameraController() {}
		virtual ~CameraController() {}

		virtual void initialize() override
		{
		}

		virtual void update(float deltaTime) override
		{
			m_camera = &getComponent<CameraComponent>();

			if (m_camera->type == CameraType::Orthographic) {
				updateOrthographic(deltaTime);
			}
			else if (m_camera->type == CameraType::Perspective) {
				updatePerspective(deltaTime);
			}
		}

		void updateOrthographic(float deltaTime)
		{
			// Update camera rotation

			float cameraRotateSpeed = ROTATE_SPEED * (1.0f/ 60.0f);

			if (Input::isKeyPressed(KeyCode("GLFW_KEY_Q"))) {
				transform->rotate.z += cameraRotateSpeed;
			}
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_E"))) {
				transform->rotate.z -= cameraRotateSpeed;
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
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_W"))) {
				transform->translate.x += -sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
				transform->translate.y +=  cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
			}
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_S"))) {
				transform->translate.x -= -sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
				transform->translate.y -=  cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
			}
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_A"))) {
				transform->translate.x -= cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
				transform->translate.y -= sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
			}
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
				transform->translate.x += cos(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
				transform->translate.y += sin(glm::radians(transform->rotate.z)) * cameraTranslateSpeed;
			}

			// Update camera zoom

			float zoomSpeed = ZOOM_SENSITIVITY * (1.0f / 60.0f);

			if (Input::isKeyPressed(KeyCode("GLFW_KEY_EQUAL"))) {
				m_camera->zoomLevel -= zoomSpeed;
			}
			if (Input::isKeyPressed(KeyCode("GLFW_KEY_MINUS"))) {
				m_camera->zoomLevel += zoomSpeed;
			}
			m_camera->zoomLevel = std::max(m_camera->zoomLevel, 0.25f);
			m_camera->zoomLevel = std::min(m_camera->zoomLevel, 10.0f);
		}

		void updatePerspective(float deltaTime)
		{

		}

	private:
		CameraComponent* m_camera;
	};

}

#endif // CAMERA_CONTROLLER_H
