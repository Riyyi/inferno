#include "inferno/application.h"
#include "inferno/assertions.h"
#include "inferno/component/transform.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/log.h"
#include "inferno/settings.h"
#include "inferno/render/buffer.h"
#include "inferno/render/camera.h"
#include "inferno/render/context.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"
#include "inferno/time.h"
#include "inferno/window.h"

namespace Inferno {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		// Initialize

		Settings::initialize();

		m_window = std::make_unique<Window>();
		m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

		Input::initialize();

		ShaderManager* shaderManager = new ShaderManager();
		shaderManager->initialize();

		TextureManager* textureManager = new TextureManager();
		textureManager->initialize();

		m_cameraO = std::make_shared<OrthographicCamera>();
		m_cameraP = std::make_shared<PerspectiveCamera>();

		Renderer2D* renderer2D = new Renderer2D();
		renderer2D->initialize();

		// Load assets

		m_texture = TextureManager::the().load("assets/gfx/test.png");
		m_texture2 = TextureManager::the().load("assets/gfx/test-inverted.png");
	}

	Application::~Application()
	{
		Renderer2D::the().destroy();
		TextureManager::the().destroy();
		ShaderManager::the().destroy();
		// Input::destroy();
		Settings::destroy();
	}

	void Application::run()
	{
		dbg() << "Application startup";

		glm::mat4 colors = {
			1.0f, 0.0f, 1.0f, 1.0f, // Lower left corner: purple
			1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		Transform cube({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
		cube.update();

		Transform cube2({1.1f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
		cube2.update();

		Transform cube3({2.2f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
		cube3.update();

		while(!m_window->shouldClose()) {

			float time = Time::time();
			float deltaTime = time - m_lastFrameTime;
			m_lastFrameTime = time;
			// dbg() << "Frametime " << deltaTime * 1000 << "ms";

			// Update

			Input::update();
			m_window->update();
			m_cameraO->update(deltaTime);
			m_cameraP->update(deltaTime);

			// Render

			RenderCommand::clearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			RenderCommand::clear();

			Renderer2D::the().beginScene(m_cameraP); // camera, lights, environment

			Renderer2D::the().drawQuad(std::make_shared<Transform>(cube), colors);
			Renderer2D::the().drawQuad(std::make_shared<Transform>(cube2), { 0.5f, 0.6f, 0.8f, 1.0f }, m_texture);
			Renderer2D::the().drawQuad(std::make_shared<Transform>(cube3), { 1.0f, 1.0f, 1.0f, 1.0f }, m_texture2);

			Renderer2D::the().endScene();

			m_window->render();

		}

		dbg() << "Application shutdown";
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(NF_BIND_EVENT(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(NF_BIND_EVENT(Application::onWindowResize));
		dispatcher.dispatch<KeyPressEvent>(NF_BIND_EVENT(Application::onKeyPress));
		dispatcher.dispatch<MousePositionEvent>(NF_BIND_EVENT(Application::onMousePosition));
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		// Suppress unused warning
		(void)e;

		dbg(Log::Info) << "WindowCloseEvent triggered";

		m_window->setShouldClose(true);

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

	bool Application::onKeyPress(KeyPressEvent& e)
	{
		// Suppress unused warning
		(void)e;

		dbgln(Log::Info, "KeyPressEvent {} ({}) triggered",
		      Input::getKeyName(e.getKey()),
		      e.getKey());

		// Stop the main loop on 'Escape' keypress
		if (e.getKey() == KeyCode("GLFW_KEY_ESCAPE")) {
			m_window->setShouldClose(true);
		}

		return true;
	}

	bool Application::onMousePosition(MousePositionEvent& e)
	{
		return Input::onMousePosition(e);
	}

}
