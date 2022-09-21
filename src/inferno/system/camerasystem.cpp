#include "glm/ext/matrix_clip_space.hpp" // glm::perspective, glm::ortho
#include "glm/ext/matrix_transform.hpp"  // glm::radians, glm::lookAt
#include "ruc/meta/assert.h"

#include "inferno/application.h"
#include "inferno/component/cameracomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/io/input.h"
#include "inferno/io/log.h"
#include "inferno/system/camerasystem.h"
#include "inferno/window.h"

namespace Inferno {

	CameraSystem::CameraSystem(s)
	{
		info() << "CameraSystem initialized";
	}

	CameraSystem::~CameraSystem()
	{
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

	glm::mat4 CameraSystem::projectionView()
	{
		auto view = m_registry->view<TransformComponent, CameraComponent>();

		for (auto [entity, transform, camera] : view.each()) {
			return camera.projection * transform.transform;
		}

		VERIFY_NOT_REACHED();

		return glm::mat4 { 1.0f };
	}

	void CameraSystem::updateOrthographic(TransformComponent& transform, CameraComponent& camera)
	{
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

} // namespace Inferno
