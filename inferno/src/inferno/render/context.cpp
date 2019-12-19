#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "inferno/core.h"
#include "inferno/log.h"
#include "inferno/render/context.h"
#include "inferno/window.h"

namespace Inferno {

	Context::Context(GLFWwindow* window) :
		m_window(window)
	{
	}

	void Context::initialize()
	{
		// Initialize glad
		glfwMakeContextCurrent(m_window);
		int glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NF_CORE_ASSERT(glad, "Failed to initialize glad!");

		// Log OpenGL properties
		NF_CORE_INFO("OpenGL Info:");
		NF_CORE_INFO("  Vendor:   %s", glGetString(GL_VENDOR));
		NF_CORE_INFO("  Renderer: %s", glGetString(GL_RENDERER));
		NF_CORE_INFO("  Version:  %s", glGetString(GL_VERSION));

		Window &w = *(Window*)glfwGetWindowUserPointer(m_window);

		// Disable vsync
		if (!w.isVSync()) {
			glfwSwapInterval(0);
		}

		// Set viewport
		glViewport(0, 0, w.getWidth(), w.getHeight());

		// Enable z-buffer / depth buffer
		glEnable(GL_DEPTH_TEST);
	}

	void Context::update()
	{
		glfwSwapBuffers(m_window);
	}

	void Context::destroy()
	{
	}

}
