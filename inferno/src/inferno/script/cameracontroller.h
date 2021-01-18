#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define TRANSLATE_SPEED 2.5f
#define ROTATE_SPEED 90.0f
#define ZOOM_SENSITIVITY 2.5f
#define MOUSE_SENSITIVITY 0.25f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#include "inferno/scene/components.h"
#include "inferno/script/nativescript.h"

namespace Inferno {

	class CameraController final : public NativeScript {
	public:

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

		void updateOrthographic(float deltaTime);
		void updatePerspective(float deltaTime);

	private:
		CameraComponent* m_camera;
	};

}

#endif // CAMERA_CONTROLLER_H
