#include <GLFW/glfw3.h>

#include "inferno/application.h"
#include "inferno/input.h"
#include "inferno/window.h"

namespace Inferno {

	bool Input::isKeyPressed(int key)
	{
		GLFWwindow* w = Application::get().getWindow().getWindow();
		return glfwGetKey(w, key) == GLFW_PRESS;
	}

	bool Input::isMouseButtonPressed(int button)
	{
		GLFWwindow* w = Application::get().getWindow().getWindow();
		return glfwGetMouseButton(w, button) == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition()
	{
		GLFWwindow* w = Application::get().getWindow().getWindow();
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

}
