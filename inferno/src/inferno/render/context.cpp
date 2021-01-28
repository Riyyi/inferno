#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "inferno/assert.h"
#include "inferno/core.h"
#include "inferno/io/log.h"
#include "inferno/render/context.h"
#include "inferno/window.h"

namespace Inferno {

	Context::Context(GLFWwindow* window) :
		m_window(window)
	{
	}

// -----------------------------------------

	void Context::initialize()
	{
		// Initialize glad
		glfwMakeContextCurrent(m_window);
		int glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(glad, "Failed to initialize glad!");

		// Log OpenGL properties
		dbg(Log::Comment) << "OpenGL Info:";
		dbg(Log::Comment) << "  Vendor:   " << glGetString(GL_VENDOR);
		dbg(Log::Comment) << "  Renderer: " << glGetString(GL_RENDERER);
		dbg(Log::Comment) << "  Version:  " << glGetString(GL_VERSION);

#ifdef NF_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
		       "Inferno requires at least OpenGL version 4.5!");
#endif

		Window& w = *(Window*)glfwGetWindowUserPointer(m_window);

		// Disable vsync
		if (!w.isVSync()) {
			glfwSwapInterval(0);
		}

		// Set viewport
		this->setViewport(0, 0, w.getWidth(), w.getHeight());

		// Enable z-buffer / depth buffer
		glEnable(GL_DEPTH_TEST);

		// Enable transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	void Context::update()
	{
	}

	void Context::render()
	{
		glfwSwapBuffers(m_window);
	}

	void Context::destroy()
	{
	}

// -----------------------------------------

	void Context::setViewport(int x, int y, int width, int height) const
	{
		glViewport(x, y, width, height);
	}
}
