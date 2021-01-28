#include "inferno/application.h"
#include "inferno/assert.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/keycodes.h"
#include "inferno/io/input.h"
#include "inferno/io/log.h"
#include "inferno/render/buffer.h"
#include "inferno/render/context.h"
#include "inferno/render/font.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"
#include "inferno/scene/scene.h"
#include "inferno/settings.h"
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

		m_scene = std::make_shared<Scene>();
		m_scene->initialize();

		Renderer2D* renderer2D = new Renderer2D();
		renderer2D->initialize();

		RendererCharacter* rendererCharacter = new RendererCharacter();
		rendererCharacter->initialize();

		FontManager* fontManager = new FontManager();
		fontManager->initialize();

		// Load assets

		m_font = FontManager::the().load("assets/fnt/dejavu-sans");
	}

	Application::~Application()
	{
		FontManager::the().destroy();
		RendererCharacter::the().destroy();
		Renderer2D::the().destroy();
		m_scene->destroy();
		TextureManager::the().destroy();
		ShaderManager::the().destroy();
		// Input::destroy();
		Settings::destroy();
	}

	void Application::run()
	{
		dbg() << "Application startup";

		std::array<CharacterVertex, Renderer::vertexPerQuad> character;

		// character.at(0).quad.textureCoordinates = { 0.0f, 0.0f }; // bottom left
		// character.at(1).quad.textureCoordinates = { 1.0f, 0.0f };
		// character.at(2).quad.textureCoordinates = { 1.0f, 1.0f }; // top right
		// character.at(3).quad.textureCoordinates = { 0.0f, 1.0f };

		auto f = FontManager::the().get("assets/fnt/dejavu-sans");
		auto c = f->get('5');
		// dbg() << c->position << " " << c->size;

		uint32_t textureWidth = f->texture()->width();
		uint32_t textureHeight = f->texture()->height();
		ASSERT(textureWidth == textureHeight, "Invalid font texture!");

		float quadWidth  = (c->size.x / (float)textureWidth) - 0.04; // @Todo something wrong with the width
		float quadHeight = c->size.y / (float)textureHeight;

		character.at(0).quad.position = { -quadWidth, -quadHeight, 0.0f }; // bottom left
		character.at(1).quad.position = {  quadWidth, -quadHeight, 0.0f }; // bottom right
		character.at(2).quad.position = {  quadWidth,  quadHeight, 0.0f }; // top right
		character.at(3).quad.position = { -quadWidth,  quadHeight, 0.0f }; // top left

		glm::vec2 x {
			1 - (textureWidth - c->position.x) / (float)textureWidth,
			1 - (textureWidth - c->position.x - c->size.x) / (float)textureWidth
		};
		glm::vec2 y {
			(textureHeight - c->position.y - c->size.y) / (float)textureHeight,
			(textureHeight - c->position.y) / (float)textureHeight
		};
		// dbg() < y;

		character.at(0).quad.textureCoordinates = { x.x, y.x };
		character.at(1).quad.textureCoordinates = { x.y, y.x };
		character.at(2).quad.textureCoordinates = { x.y, y.y };
		character.at(3).quad.textureCoordinates = { x.x, y.y };

		// pos
		// texcoords
		//
		// width
		// edge
		// borderwidth
		// borderedge
		// bordercolor
		// offse

		while (!m_window->shouldClose()) {

			float time = Time::time();
			float deltaTime = time - m_lastFrameTime;
			m_lastFrameTime = time;
			// dbg() << "Frametime " << deltaTime * 1000 << "ms";

			// Update

			Input::update();
			m_window->update();
			m_scene->update(deltaTime);

			// Render

			RenderCommand::clearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			RenderCommand::clear();

			Renderer2D::the().beginScene(m_scene->cameraProjectionView()); // camera, lights, environment
			RendererCharacter::the().beginScene();

			m_scene->render();
			RendererCharacter::the().drawCharacter(character, f->texture());

			Renderer2D::the().endScene();
			RendererCharacter::the().endScene();

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
		if (e.getKey() == keyCode("GLFW_KEY_ESCAPE")) {
			m_window->setShouldClose(true);
		}

		return true;
	}

	bool Application::onMousePosition(MousePositionEvent& e)
	{
		return Input::onMousePosition(e);
	}

}
