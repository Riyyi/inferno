#include <glm/ext/matrix_clip_space.hpp> // glm::perspective, glm::ortho
#include <glm/ext/matrix_transform.hpp>  // glm::radians, glm::lookAt

#include "inferno/application.h"
#include "inferno/assertions.h"
#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/log.h"
#include "inferno/scene/entity.h"
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
		auto orthoView = m_registry->view<TransformComponent, OrthographicCameraComponment>();

		for(auto&& [entity, transform, orthographic] : orthoView.each()) {
			updateOrthographic(transform, orthographic);
		}

		auto perspectiveView = m_registry->view<TransformComponent, PerspectiveCameraComponent>();

		for(auto&& [entity, transform, perspective] : perspectiveView.each()) {
			updatePerspective(transform, perspective);
		}
	}

	void CameraSystem::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	glm::mat4 CameraSystem::cameraProjectionView()
	{
		auto orthoView = m_registry->view<TransformComponent, OrthographicCameraComponment>();

		for(auto&& [entity, transform, orthographic] : orthoView.each()) {
			return orthographic.projection * transform.transform;
		}

		auto perspectiveView = m_registry->view<TransformComponent, PerspectiveCameraComponent>();

		for(auto&& [entity, transform, perspective] : perspectiveView.each()) {
			return perspective.projection * transform.transform;
		}

		ASSERT_NOT_REACHED();

		return glm::mat4 { 1.0f };
	}

	void CameraSystem::updateOrthographic(TransformComponent& transform, OrthographicCameraComponment& orthographic)
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
			orthographic.zoomLevel -= zoomSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_MINUS"))) {
			orthographic.zoomLevel += zoomSpeed;
		}
		orthographic.zoomLevel = std::max(orthographic.zoomLevel, 0.25f);
		orthographic.zoomLevel = std::min(orthographic.zoomLevel, 10.0f);

		// Update camera matrix

		// Local space -> World space: model matrix
		// Is done in Object::update()

		// World space -> View space: view matrix
		transform.transform = {
			glm::translate(glm::mat4(1.0f), transform.translate) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotate.z), orthographic.rotateAxis)
		};
		transform.transform = { glm::inverse(transform.transform) };

		// View space -> Clip space: projection matrix
		float aspectRatio = Application::the().getWindow().getAspect();
		orthographic.projection = {
			glm::ortho(-aspectRatio * orthographic.zoomLevel, aspectRatio * orthographic.zoomLevel,
			           -orthographic.zoomLevel, orthographic.zoomLevel, -1.0f, 1.0f)
		};

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport
	}

	void CameraSystem::updatePerspective(TransformComponent& transform, PerspectiveCameraComponent& perspective)
	{
		// Get mouse movement offset compared to last frame
		float xOffset = Input::getXOffset() * MOUSE_SENSITIVITY;
		float yOffset = Input::getYOffset() * MOUSE_SENSITIVITY;
		perspective.yaw += xOffset;
		perspective.pitch += yOffset;

		// Prevent gimbal lock
		if (perspective.pitch > 89.0f) perspective.pitch = 89.0f;
		if (perspective.pitch < -89.0f) perspective.pitch = -89.0f;

		// Update camera rotation, by calculating direction vector via yaw and pitch

		transform.rotate = {
			cos(glm::radians(perspective.pitch)) * cos(glm::radians(perspective.yaw)),
			sin(glm::radians(perspective.pitch)),
			cos(glm::radians(perspective.pitch)) * sin(glm::radians(perspective.yaw))
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
				glm::normalize(glm::cross(transform.rotate, perspective.up)) * cameraSpeed };
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
			transform.translate = { transform.translate +
				glm::normalize(glm::cross(transform.rotate, perspective.up)) * cameraSpeed };
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
		transform.transform = { glm::lookAt(transform.translate, transform.translate + transform.rotate, perspective.up) };

		// View space -> Clip space: projection matrix
		float aspect = Application::the().getWindow().getAspect();
		perspective.projection = { glm::perspective(glm::radians(perspective.fov), aspect, NEAR_PLANE, FAR_PLANE) };

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport

		// Souce: https://learnopengl.com/img/getting-started/coordinate_systems.png
	}

}
