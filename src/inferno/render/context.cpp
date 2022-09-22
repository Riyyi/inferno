// clang-format off
#include "glad/glad.h" // glad needs to come before GLFW
#include "GLFW/glfw3.h"
// clang-format on
#include "ruc/meta/assert.h"

#include "inferno/core.h"
#include "inferno/io/log.h"
#include "inferno/render/context.h"
#include "inferno/window.h"

namespace Inferno {

Context::Context(GLFWwindow* window)
	: m_window(window)
{
	VERIFY(window, "Context window is nullptr!");
}

void Context::initialize()
{
	Context::setCurrent();

	// Initialize glad
	int glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	VERIFY(glad, "Failed to initialize glad!");

	// Log OpenGL properties
	comment() << "OpenGL Info:";
	comment() << "  Vendor:   " << glGetString(GL_VENDOR);
	comment() << "  Renderer: " << glGetString(GL_RENDERER);
	comment() << "  Version:  " << glGetString(GL_VERSION);

	// Check OpenGL version
	VERIFY(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
	       "Inferno requires at least OpenGL version 4.5!");
}

void Context::destroy()
{
}

void Context::render()
{
	glfwSwapBuffers(m_window);
}

void Context::setCurrent()
{
	// Set current OpenGL context to this window
	glfwMakeContextCurrent(m_window);
}

} // namespace Inferno
