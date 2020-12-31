#include <GLFW/glfw3.h>

#include "inferno/application.h"
#include "inferno/assertions.h"
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
#include "inferno/render/texture.h"
#include "inferno/window.h"

namespace Inferno {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		// Initialize Settings
		Settings::initialize();

		m_window = std::make_unique<Window>();
		m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

		TextureManager textureManager;
		m_texture = textureManager.load("assets/gfx/test.png");

// -----------------------------------------

		float verticesColor[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};

		uint32_t indicesColor[] = {
			0, 1, 2, 2, 3, 0
		};

		m_vertexArrayColor = std::make_shared<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBufferColor = std::make_shared<VertexBuffer>(verticesColor, sizeof(verticesColor));
		vertexBufferColor->setLayout({
			{ BufferElementType::Vec3, "a_position" },
			{ BufferElementType::Vec4, "a_color" },
		});

		m_vertexArrayColor->addVertexBuffer(vertexBufferColor);

		std::shared_ptr<IndexBuffer> indexBufferColor = std::make_shared<IndexBuffer>(indicesColor, sizeof(indicesColor));

		m_vertexArrayColor->setIndexBuffer(indexBufferColor);

// -----------------------------------------

		m_vertexArrayTexture = std::make_shared<VertexArray>();

		float verticesTexture[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		uint32_t indicesTexture[] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<VertexBuffer> vertexBufferTexture = std::make_shared<VertexBuffer>(verticesTexture, sizeof(verticesTexture));
		vertexBufferTexture->setLayout({
			{ BufferElementType::Vec3, "a_position" },
			{ BufferElementType::Vec2, "a_texCoord" },
		});

		m_vertexArrayTexture->addVertexBuffer(vertexBufferTexture);

		std::shared_ptr<IndexBuffer> indexBufferTexture = std::make_shared<IndexBuffer>(indicesTexture, sizeof(indicesTexture));

		m_vertexArrayTexture->setIndexBuffer(indexBufferTexture);

// -----------------------------------------

		ShaderManager shaderManager;
		m_shaderSimple = shaderManager.load("assets/glsl/simple");
		m_shaderTexture = shaderManager.load("assets/glsl/texture");
		m_shaderTexture->setInt("u_texture", m_texture->id());
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		dbg() << "Application startup";

		while(!glfwWindowShouldClose(m_window->getWindow())) {

			if(glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);
			}

			Command::clearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			Command::clear();

			// Renderer::beginScene(); // camera, lights, environment

			// m_shaderSimple->bind();
			// Renderer::submit(m_vertexArrayColor);
			// m_shaderSimple->unbind();

			m_shaderTexture->bind();
			m_texture->bind();
			Renderer::submit(m_vertexArrayTexture);
			m_texture->unbind();
			m_shaderTexture->unbind();

			// Renderer::endScene();

			m_window->update();
		}

		dbg() << "Application shutdown";
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(NF_BIND_EVENT(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(NF_BIND_EVENT(Application::onWindowResize));
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		// Suppress unused warning
		(void)e;

		dbg(Log::Info) << "WindowCloseEvent triggered";

		glfwSetWindowShouldClose(m_window->getWindow(), GL_TRUE);

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		// Suppress unused warning
		(void)e;

		dbgln(Log::Info, "WindowResizeEvent {}x{} triggered", e.getWidth(), e.getHeight());

		m_window->getContext()->setViewport(0, 0, e.getWidth(), e.getHeight());

		return true;
	}

}
