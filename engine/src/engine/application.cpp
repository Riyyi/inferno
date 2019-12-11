#include <cstdio> // printf

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "log.h"

namespace Engine {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		LOG_ENGINE_LOG("Startup!");

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* window = glfwCreateWindow(1280, 720, "Engine", NULL, NULL);
		if (window == NULL) {
			LOG_ENGINE_DANGER("Failed to create GLFW window");
			glfwTerminate();
			return;// -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_ENGINE_DANGER("Failed to initialize GLAD");
			return;// -1;
		}

		glViewport(0, 0, 1280, 720);

		while(!glfwWindowShouldClose(window)) {

			if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		glfwTerminate();

		LOG_ENGINE_LOG("Shutdown!");
	}

}
