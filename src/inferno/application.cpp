#include "glm/gtc/type_ptr.hpp" // glm::make_mat4

#include "inferno/application.h"
#include "inferno/assert.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/io/gltffile.h"
#include "inferno/io/input.h"
#include "inferno/io/log.h"
#include "inferno/keycodes.h"
#include "inferno/render/buffer.h"
#include "inferno/render/context.h"
#include "inferno/render/font.h"
#include "inferno/render/gltf.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"
#include "inferno/scene/scene.h"
#include "inferno/settings.h"
#include "inferno/time.h"
#include "inferno/window.h"
#include <string>

namespace Inferno {

	Application::Application(s)
	{
		// Set singleton instance early
		s_instance = this;

		// Initialize

		Settings::initialize();

		m_window = std::make_unique<Window>();
		m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

		Input::initialize();
		ShaderManager::initialize();
		TextureManager::initialize();
		RenderCommand::initialize();
		Renderer2D::initialize();
		RendererCharacter::initialize();
		FontManager::initialize();

		m_scene = std::make_shared<Scene>();
		m_scene->initialize();

		// Load assets

		m_font = FontManager::the().load("assets/fnt/dejavu-sans");

		// auto bla = GlTFFile::read("assets/gltf/box.glb");
		// success() << "@" << bla.first.get() << "@";
		// auto bla2 = GlTFFile::read("assets/gltf/boxtextured.glb");
		// info() << "@" << bla2.first.get() << "@";
		// auto bla3 = GlTFFile::read("assets/gltf/guinea-pig-cage-fleece.glb");
		// warn() << "@" << bla3.first.get() << "@";

		// Gltf model = Gltf("assets/gltf/box.glb");

		// Gltf model = Gltf("assets/gltf/animatedmorphcube.glb");
		// Gltf model = Gltf("assets/gltf/reciprocatingsaw.glb");

		Gltf model = Gltf("assets/gltf/triangle-without-indices.gltf");
	}

	Application::~Application()
	{
		m_scene->destroy();

		FontManager::destroy();
		RendererCharacter::destroy();
		Renderer2D::destroy();
		RenderCommand::destroy();
		TextureManager::destroy();
		ShaderManager::destroy();
		// Input::destroy();

		Settings::destroy();
	}

	int Application::run()
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
		// offset

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
			// RendererCharacter::the().drawCharacter(character, f->texture());

			Renderer2D::the().endScene();
			RendererCharacter::the().endScene();

			m_window->render();

		}

		dbg() << "Application shutdown";

		return m_status;
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

		info() << "WindowCloseEvent triggered";
		infoln("{}Event triggered", e.toString());

		m_window->setShouldClose(true);

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		// Suppress unused warning
		(void)e;

		infoln("WindowResizeEvent {}x{} triggered", e.getWidth(), e.getHeight());

		RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());

		return true;
	}

	bool Application::onKeyPress(KeyPressEvent& e)
	{
		// Suppress unused warning
		(void)e;

		infoln("KeyPressEvent {} ({}) triggered",
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
