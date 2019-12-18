#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/log.h"
#include "inferno/settings.h"
#include "inferno/window.h"

namespace Inferno {

	unsigned char Window::s_windowCount = 0;

	Window::Window()
	{
		m_windowProperties = {
			Settings::get().properties().title,
			Settings::get().properties().width,
			Settings::get().properties().height,
		};

		this->initialize();
	}

	Window::~Window()
	{
		this->destroy();
	}

	void Window::initialize()
	{
		const char* title = m_windowProperties.title;
		unsigned int width = m_windowProperties.width;
		unsigned int height = m_windowProperties.height;

		// Only init once
		if (s_windowCount == 0) {
			glfwInit();
		}

		// Set window properties
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Create GLFW window
		m_window = glfwCreateWindow(width, height, title, NULL, NULL);
		s_windowCount++;
		NF_CORE_ASSERT(m_window, "Failed to create GLFW window!");

		// Initialize glad
		glfwMakeContextCurrent(m_window);
		int glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NF_CORE_ASSERT(glad, "Failed to initialize glad!");

		// Log OpenGL properties
		NF_CORE_INFO("OpenGL Info:");
		NF_CORE_INFO("  Vendor:   %s", glGetString(GL_VENDOR));
		NF_CORE_INFO("  Renderer: %s", glGetString(GL_RENDERER));
		NF_CORE_INFO("  Version:  %s", glGetString(GL_VERSION));

		// Capture cursor and hide it
		// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Associate the wrapper to the window
		glfwSetWindowUserPointer(m_window, this);

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
		// glfwSetKeyCallback

		// Mouse position callback
		// glfwSetCursorPosCallback

		// Mouse button callback
		// glfwSetMouseButtonCallback

		// Mouse scroll callback
		// glfwSetScrollCallback

		glViewport(0, 0, width, height);

		// Enable z-buffer / depth buffer
		glEnable(GL_DEPTH_TEST);
	}

	void Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
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
