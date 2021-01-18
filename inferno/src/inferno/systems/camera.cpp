#include <glm/ext/matrix_clip_space.hpp> // glm::perspective, glm::ortho
#include <glm/ext/matrix_transform.hpp>  // glm::radians, glm::lookAt

#include "inferno/application.h"
#include "inferno/assertions.h"
#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/log.h"
#include "inferno/systems/camera.h"
#include "inferno/window.h"

namespace Inferno {

	CameraSystem* CameraSystem::s_instance = nullptr;

	void CameraSystem::initialize()
	{
		ASSERT(!s_instance, "CameraSystem already exists!");
		s_instance = this;

		dbg(Log::Info) << "CameraSystem initialized";
	}

	void CameraSystem::update()
	{
		auto view = m_registry->view<TransformComponent, CameraComponent>();

		for (auto [entity, transform, camera] : view.each()) {

			if (camera.type == CameraType::Orthographic) {
				updateOrthographic(transform, camera);
			}
			else if (camera.type == CameraType::Perspective) {
				updatePerspective(transform, camera);
			}
		}
	}

	void CameraSystem::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	glm::mat4 CameraSystem::projectionView()
	{
		auto view = m_registry->view<TransformComponent, CameraComponent>();

		for (auto [entity, transform, camera] : view.each()) {
			return camera.projection * transform.transform;
		}

		ASSERT_NOT_REACHED();

		return glm::mat4 { 1.0f };
	}

	void CameraSystem::updateOrthographic(TransformComponent& transform, CameraComponent& camera)
	{
		// Update camera rotation

		float cameraRotateSpeed = ROTATE_SPEED * (1.0f/ 60.0f);

		if (Input::isKeyPressed(KeyCode("GLFW_KEY_Q"))) {
			transform.rotate.z += cameraRotateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_E"))) {
			transform.rotate.z -= cameraRotateSpeed;
		}

		if (transform.rotate.z > 180.0f) {
			transform.rotate.z -= 360.0f;
		}
		else if (transform.rotate.z <= -180.0f) {
			transform.rotate.z += 360.0f;
		}

		// Update camera translation

		float cameraTranslateSpeed = TRANSLATE_SPEED * (1.0f / 60.0f);

		// WASD movement
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_W"))) {
			transform.translate.x += -sin(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
			transform.translate.y +=  cos(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_S"))) {
			transform.translate.x -= -sin(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
			transform.translate.y -=  cos(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_A"))) {
			transform.translate.x -= cos(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
			transform.translate.y -= sin(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
			transform.translate.x += cos(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
			transform.translate.y += sin(glm::radians(transform.rotate.z)) * cameraTranslateSpeed;
		}

		// Update camera zoom

		float zoomSpeed = ZOOM_SENSITIVITY * (1.0f / 60.0f);

		if (Input::isKeyPressed(KeyCode("GLFW_KEY_EQUAL"))) {
			camera.zoomLevel -= zoomSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_MINUS"))) {
			camera.zoomLevel += zoomSpeed;
		}
		camera.zoomLevel = std::max(camera.zoomLevel, 0.25f);
		camera.zoomLevel = std::min(camera.zoomLevel, 10.0f);

		// Update camera matrix

		// Local space -> World space: model matrix
		// Is done in Object::update()

		// World space -> View space: view matrix
		transform.transform = {
			glm::translate(glm::mat4(1.0f), transform.translate) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotate.z), camera.rotateAxis)
		};
		transform.transform = { glm::inverse(transform.transform) };

		// View space -> Clip space: projection matrix
		float aspectRatio = Application::the().getWindow().getAspect();
		camera.projection = {
			glm::ortho(-aspectRatio * camera.zoomLevel, aspectRatio * camera.zoomLevel,
			           -camera.zoomLevel, camera.zoomLevel, -1.0f, 1.0f)
		};

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport
	}

	void CameraSystem::updatePerspective(TransformComponent& transform, CameraComponent& camera)
	{
		// Get mouse movement offset compared to last frame
		float xOffset = Input::getXOffset() * MOUSE_SENSITIVITY;
		float yOffset = Input::getYOffset() * MOUSE_SENSITIVITY;
		camera.yaw += xOffset;
		camera.pitch += yOffset;

		// Prevent gimbal lock
		if (camera.pitch > 89.0f) camera.pitch = 89.0f;
		if (camera.pitch < -89.0f) camera.pitch = -89.0f;

		// Update camera rotation, by calculating direction vector via yaw and pitch

		transform.rotate = {
			cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw)),
			sin(glm::radians(camera.pitch)),
			cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw))
		};
		transform.rotate = glm::normalize(transform.rotate);
		// The direction vector is based on
		// Camera direction (z): normalize(position - target)
		// Right axis       (x): normalize(cross(up, direction))
		// Up axis          (y): cross(direction, right)

		// Source: https://learnopengl.com/img/getting-started/camera_axes.png

		// Cross = combination of two vectors in 3D space,
		//         where result is always perpendicular to both of the vectors

		// Update camera translation

		float cameraSpeed = TRANSLATE_SPEED * (1.0f / 60.0f);
		// WASD movement
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_W"))) {
			transform.translate = { transform.translate + cameraSpeed * transform.rotate };
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_S"))) {
			transform.translate = { transform.translate - cameraSpeed * transform.rotate };
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_A"))) {
			transform.translate = { transform.translate -
				glm::normalize(glm::cross(transform.rotate, camera.up)) * cameraSpeed };
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
			transform.translate = { transform.translate +
				glm::normalize(glm::cross(transform.rotate, camera.up)) * cameraSpeed };
		}
		// Up / down movement
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_SPACE"))) {
			transform.translate = { transform.translate.x, transform.translate.y + cameraSpeed, transform.translate.z };
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_LEFT_SHIFT"))) {
			transform.translate = { transform.translate.x, transform.translate.y - cameraSpeed, transform.translate.z };
		}

		// Update camera matrix

		// Local space -> World space: model matrix
		// Is done in Object::update()

		// World space -> View space: view matrix
		transform.transform = { glm::lookAt(transform.translate, transform.translate + transform.rotate, camera.up) };

		// View space -> Clip space: projection matrix
		float aspect = Application::the().getWindow().getAspect();
		camera.projection = { glm::perspective(glm::radians(camera.fov), aspect, NEAR_PLANE, FAR_PLANE) };

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport

		// Souce: https://learnopengl.com/img/getting-started/coordinate_systems.png
	}

}
