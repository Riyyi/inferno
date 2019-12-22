#include <GLFW/glfw3.h>

#include "inferno/application.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/input.h"
#include "inferno/log.h"
#include "inferno/settings.h"
#include "inferno/render/buffer.h"
#include "inferno/render/context.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/window.h"

namespace Inferno {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		NF_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		// Initialize Settings
		new Settings();

		m_window = std::make_unique<Window>();
		m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		m_vertexArray = std::make_shared<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
		vertexBuffer->setLayout({
			{ BufferElementType::Vec3, "a_position" },
			{ BufferElementType::Vec4, "a_color" },
		});

		m_vertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices));

		m_vertexArray->setIndexBuffer(indexBuffer);

		m_shader = std::make_unique<Shader>("assets/glsl/simple.vert", "assets/glsl/simple.frag");
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		NF_CORE_LOG("Application startup");

		while(!glfwWindowShouldClose(m_window->getWindow())) {

			if(glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);
			}

			Command::clearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			Command::clear();

			Renderer::beginScene(); // camera, lights, environment
			m_shader->bind();
			Renderer::submit(m_vertexArray);
			Renderer::endScene();

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
		// Suppress unused warning
		(void)e;

		NF_CORE_INFO("WindowCloseEvent triggered");

		glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent &e)
	{
		// Suppress unused warning
		(void)e;

		NF_CORE_INFO("WindowResizeEvent %dx%d triggered", e.getWidth(), e.getHeight());

		m_window->getContext()->setViewport(0, 0, e.getWidth(), e.getHeight());

		return true;
	}

}
