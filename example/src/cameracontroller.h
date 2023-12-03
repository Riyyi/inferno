/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "inferno/component/cameracomponent.h"
#include "inferno/script/nativescript.h"

#define TRANSLATE_SPEED 2.5f
#define ROTATE_SPEED 90.0f
#define ZOOM_SENSITIVITY 2.5f
#define MOUSE_SENSITIVITY 0.25f

namespace example {

class CameraController final : public Inferno::NativeScript {
public:
	virtual void update(float deltaTime) override
	{
		m_camera = &getComponent<Inferno::CameraComponent>();

		if (m_camera->type == Inferno::CameraType::Orthographic) {
			updateOrthographic(deltaTime);
		}
		else if (m_camera->type == Inferno::CameraType::Perspective) {
			updatePerspective(deltaTime);
		}
	}

	void updateOrthographic(float deltaTime);
	void updatePerspective(float deltaTime);

private:
	Inferno::CameraComponent* m_camera { nullptr };
};

BIND_NATIVE(CameraController);

} // namespace example
