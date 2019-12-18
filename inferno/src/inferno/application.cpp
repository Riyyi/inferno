#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "inferno/application.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/log.h"
#include "inferno/settings.h"
#include "inferno/window.h"

namespace Inferno {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		NF_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		NF_CORE_LOG("Application startup");

		// Initialize Settings
		new Settings();

		m_window = std::make_unique<Window>();
		m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

		while(!glfwWindowShouldClose(m_window->getWindow())) {

			if(glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_window->update();
		}

		NF_CORE_LOG("Application shutdown");
	}

	void Application::onEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(NF_BIND_EVENT(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(NF_BIND_EVENT(Application::onWindowResize));
	}

	bool Application::onWindowClose(WindowCloseEvent &e)
	{
		(void)e;
		NF_CORE_INFO("WindowCloseEvent triggered");

		glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent &e)
	{
		(void)e;
		NF_CORE_INFO("WindowResizeEvent %dx%d triggered", e.getWidth(), e.getHeight());

		return true;
	}

}
