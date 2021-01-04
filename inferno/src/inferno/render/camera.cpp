#include <memory> // std::make_shared

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective, glm::ortho
#include <glm/ext/matrix_transform.hpp>  // glm::radians
#include <glm/gtx/transform.hpp>         // glm::lookAt, glm::perspective

#include "inferno/application.h"
#include "inferno/component/transform.h"
#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/render/camera.h"
#include "inferno/window.h"

namespace Inferno {

	Camera::Camera()
		: Camera({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f})
	{
	}

	Camera::Camera(glm::vec3 translate, glm::vec3 rotate)
	{
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
		m_transform = std::make_shared<Transform>(translate, rotate, scale);
	}

// ----------------------------------------

	OrthographicCamera::OrthographicCamera()
		: Camera({0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f})
	{
		initialize();
	}

	OrthographicCamera::OrthographicCamera(glm::vec3 translate, glm::vec3 rotate)
		: Camera(translate, rotate)
	{
		initialize();
	}

	void OrthographicCamera::initialize()
	{
		// Set the roatation axis
		m_rotate = glm::vec3(0.0f, 0.0f, 1.0f);

		dbg(Log::Info) << "OrthographicCamera initialized";
	}

	void OrthographicCamera::update(float deltaTime)
	{
		// Update camera rotation

		float cameraRotateSpeed = ROTATE_SPEED * deltaTime;

		glm::vec3 rotate = transform()->rotate();
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_Q"))) {
			rotate.z += cameraRotateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_E"))) {
			rotate.z -= cameraRotateSpeed;
		}

		if (rotate.z > 180.0f)
			rotate.z -= 360.0f;
		else if (rotate.z <= -180.0f)
			rotate.z += 360.0f;

		transform()->setRotate(rotate);

		// Update camera translation

		float cameraTranslateSpeed = TRANSLATE_SPEED * deltaTime;

		glm::vec3 translate = transform()->translate();
		// WASD movement
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_W"))) {
			translate.x += -sin(glm::radians(rotate.z)) * cameraTranslateSpeed;
			translate.y +=  cos(glm::radians(rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_S"))) {
			translate.x -= -sin(glm::radians(rotate.z)) * cameraTranslateSpeed;
			translate.y -=  cos(glm::radians(rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_A"))) {
			translate.x -= cos(glm::radians(rotate.z)) * cameraTranslateSpeed;
			translate.y -= sin(glm::radians(rotate.z)) * cameraTranslateSpeed;
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
			translate.x += cos(glm::radians(rotate.z)) * cameraTranslateSpeed;
			translate.y += sin(glm::radians(rotate.z)) * cameraTranslateSpeed;
		}

		transform()->setTranslate(translate);

		// Update camera matrix

		// Local space -> World space: model matrix
		// Is done in Object::update()

		// World space -> View space: view matrix
		transform()->setTransform(glm::translate(glm::mat4(1.0f), translate) * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), m_rotate));
		transform()->setTransform(glm::inverse(transform()->transform()));

		// View space -> Clip space: projection matrix
		float aspectRatio = Application::get().getWindow().getAspect();
		setProjection(glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f));

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport
	}

// ----------------------------------------

	PerspectiveCamera::PerspectiveCamera()
		: Camera({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f})
	{
		initialize();
	}

	PerspectiveCamera::PerspectiveCamera(glm::vec3 translate, glm::vec3 rotate)
		: Camera(translate, rotate)
	{
		initialize();
	}

	void PerspectiveCamera::initialize()
	{
		m_fov = 90.0f;
		m_pitch = 0.0f;
		m_yaw = -90.0f;

		// Up vector in world space
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		dbg(Log::Info) << "PerspectiveCamera initialized";
	}

	void PerspectiveCamera::update(float deltaTime)
	{
		// Get mouse movement offset compared to last frame
		float xOffset = Input::getXOffset() * SENSITIVITY;
		float yOffset = Input::getYOffset() * SENSITIVITY;
		m_yaw += xOffset;
		m_pitch += yOffset;

		// Prevent gimbal lock
		if (m_pitch > 89.0f) m_pitch = 89.0f;
		if (m_pitch < -89.0f) m_pitch = -89.0f;

		// Update camera rotation, by calculating direction vector via yaw and pitch

		transform()->setRotate(
			cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw)),
			sin(glm::radians(m_pitch)),
			cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw)));
		transform()->setRotate(glm::normalize(transform()->rotate()));
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
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_W"))) {
			transform()->setTranslate(transform()->translate() + cameraSpeed * transform()->rotate());
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_S"))) {
			transform()->setTranslate(transform()->translate() - cameraSpeed * transform()->rotate());
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_A"))) {
			transform()->setTranslate(transform()->translate() -
						glm::normalize(glm::cross(transform()->rotate(), m_up)) * cameraSpeed);
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_D"))) {
			transform()->setTranslate(transform()->translate() +
						glm::normalize(glm::cross(transform()->rotate(), m_up)) * cameraSpeed);
		}
		// Up / down movement
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_SPACE"))) {
			transform()->setTranslate(transform()->translate().x, transform()->translate().y + cameraSpeed, transform()->translate().z);
		}
		if (Input::isKeyPressed(KeyCode("GLFW_KEY_LEFT_SHIFT"))) {
			transform()->setTranslate(transform()->translate().x, transform()->translate().y - cameraSpeed, transform()->translate().z);
		}

		// Update camera matrix

		// Local space -> World space: model matrix
		// Is done in Object::update()

		// World space -> View space: view matrix
		transform()->setTransform(glm::lookAt(transform()->translate(), transform()->translate() + transform()->rotate(), m_up));

		// View space -> Clip space: projection matrix
		float aspect = Application::get().getWindow().getAspect();
		setProjection(glm::perspective(glm::radians(m_fov), aspect, NEAR_PLANE, FAR_PLANE));

		// Clip space -> Screen space: viewport transform
		// Is done in the fragment shader using the settings of glViewport

		// Souce: https://learnopengl.com/img/getting-started/coordinate_systems.png
	}

}
