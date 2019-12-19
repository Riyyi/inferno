#include <cstring>

#include <GLFW/glfw3.h>

#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/log.h"
#include "inferno/render/context.h"
#include "inferno/settings.h"
#include "inferno/window.h"

namespace Inferno {

	unsigned char Window::s_windowCount = 0;

	Window::Window()
	{
		m_windowProperties = {
			Settings::get().properties().window.title,
			Settings::get().properties().window.width,
			Settings::get().properties().window.height,
			Settings::get().properties().window.fullscreen,
			Settings::get().properties().window.vsync,
		};

		this->initialize();
	}

	Window::~Window()
	{
		this->destroy();
	}

	void Window::initialize()
	{
		const char* title      = m_windowProperties.title;
		unsigned int width     = m_windowProperties.width;
		unsigned int height    = m_windowProperties.height;
		const char* fullscreen = m_windowProperties.fullscreen;
		bool vsync             = m_windowProperties.vsync;

		// Only init once
		if (s_windowCount == 0) {
			glfwInit();
		}

		// Set window properties
		// -----------------------------------------
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Windowed
		GLFWmonitor* monitor = nullptr;
		// Fullscreen
		if (strcmp(fullscreen, "fullscreen") == 0) {
			monitor = glfwGetPrimaryMonitor();
		}
		// Borderless fullscreen
		if (strcmp(fullscreen, "borderless") == 0) {
			monitor = glfwGetPrimaryMonitor();

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			width = mode->width;
			height = mode->height;

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		}

		// Vsync
		if (!vsync) {
			glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
		}
		// -----------------------------------------

		// Create GLFW window
		m_window = glfwCreateWindow(width, height, title, monitor, nullptr);
		s_windowCount++;
		NF_CORE_ASSERT(m_window, "Failed to create GLFW window!");

		// Associate the wrapper to the window
		glfwSetWindowUserPointer(m_window, this);

		// Create graphics context
		m_context = new Context(m_window);
		m_context->initialize();

		// Capture cursor and hide it
		// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Error callback
		glfwSetErrorCallback([](int error, const char* description) {
			NF_CORE_LOG("GLFW Error %d: %s", error, description);
		});

		// Window close callback
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			w.m_eventCallback(event);
		});

		// Window resize callback
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			w.m_windowProperties.width = width;
			w.m_windowProperties.height = height;

			WindowResizeEvent event(width, height);
			w.m_eventCallback(event);
		});

		// Keyboard callback
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
			// Suppress unused warning
			(void)scanCode;
			(void)mods;

			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressEvent event(key);
					w.m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleaseEvent event(key);
					w.m_eventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyRepeatEvent event(key);
					w.m_eventCallback(event);
					break;
				}
			}
		});

		// Mouse button callback
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			// Suppress unused warning
			(void)mods;

			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressEvent event(button);
					w.m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleaseEvent event(button);
					w.m_eventCallback(event);
					break;
				}
			}
		});

		// Mouse position callback
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			MousePositionEvent event(xPos, yPos);
			w.m_eventCallback(event);
		});

		// Mouse scroll callback
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			Window &w = *(Window*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event(xOffset, yOffset);
			w.m_eventCallback(event);
		});
	}

	void Window::update()
	{
		glfwPollEvents();
		m_context->update();
	}

	void Window::destroy()
	{
		glfwDestroyWindow(m_window);
		s_windowCount--;

		if (s_windowCount == 0) {
			glfwTerminate();
		}
	}

}
