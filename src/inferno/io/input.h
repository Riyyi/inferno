/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <utility> // std::pair

namespace Inferno {

class MousePositionEvent;

class Input {
public:
	static void initialize();
	static void update();

	static bool onMousePosition(MousePositionEvent& e);

	static bool isKeyPressed(int key);
	static bool isMouseButtonPressed(int button);
	static std::pair<float, float> getMousePosition();
	static float getMouseX();
	static float getMouseY();

	static const char* getKeyName(int key);
	static int getKeyScancode(int key);

	static inline float getXOffset() { return m_xOffset; }
	static inline float getYOffset() { return m_yOffset; }

private:
	static bool m_firstMousePos;
	static float m_xPosLast;
	static float m_yPosLast;
	static float m_xOffset;
	static float m_yOffset;
};

} // namespace Inferno
