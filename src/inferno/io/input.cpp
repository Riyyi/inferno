/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "GLFW/glfw3.h"
#include "ruc/format/log.h"

#include "inferno/application.h"
#include "inferno/event/mouseevent.h"
#include "inferno/io/input.h"
#include "inferno/window.h"

namespace Inferno {

bool Input::m_firstMousePos = true;
float Input::m_xPosLast = 0.0f;
float Input::m_yPosLast = 0.0f;
float Input::m_xOffset = 0.0f;
float Input::m_yOffset = 0.0f;

void Input::initialize()
{
	// Set cursor in the middle of the screen
	m_xPosLast = Application::the().getWindow().getWidth() / 2.0f;
	m_yPosLast = Application::the().getWindow().getHeight() / 2.0f;

	ruc::info("Input initialized");
}

void Input::update()
{
	// Stop infinite mouse movement
	m_xOffset = 0.0f;
	m_yOffset = 0.0f;
}

bool Input::onMousePosition(MousePositionEvent& e)
{
	// Prevent weird jump on first cursor window enter
	if (m_firstMousePos) {
		m_firstMousePos = false;
		m_xPosLast = e.getXPos();
		m_yPosLast = e.getYPos();
	}

	m_xOffset = e.getXPos() - m_xPosLast;
	// Reversed since y-coordinates range from bottom to top
	m_yOffset = m_yPosLast - e.getYPos();
	m_xPosLast = e.getXPos();
	m_yPosLast = e.getYPos();

	return true;
}

bool Input::isKeyPressed(int key)
{
	GLFWwindow* w = Application::the().getWindow().getWindow();
	return glfwGetKey(w, key) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(int button)
{
	GLFWwindow* w = Application::the().getWindow().getWindow();
	return glfwGetMouseButton(w, button) == GLFW_PRESS;
}

std::pair<float, float> Input::getMousePosition()
{
	GLFWwindow* w = Application::the().getWindow().getWindow();
	double xPos;
	double yPos;
	glfwGetCursorPos(w, &xPos, &yPos);
	return { (float)xPos, (float)yPos };
}

float Input::getMouseX()
{
	return getMousePosition().first;
}

float Input::getMouseY()
{
	return getMousePosition().second;
}

const char* Input::getKeyName(int key)
{
	return glfwGetKeyName(key, getKeyScancode(key));
}

int Input::getKeyScancode(int key)
{
	return glfwGetKeyScancode(key);
}

} // namespace Inferno
