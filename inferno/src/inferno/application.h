#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory> // std::unique_ptr, std::shared_ptr

#include "inferno/singleton.h"

namespace Inferno {

	class Event;
	class Font;
	class KeyPressEvent;
	class MousePositionEvent;
	class Scene;
	class Window;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class Application : public Singleton<Application> {
	public:
		Application(s);
		virtual ~Application();

		int run();

		void onEvent(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onKeyPress(KeyPressEvent& e);
		bool onMousePosition(MousePositionEvent& e);

		inline void setStatus(int status) { m_status = status; }

		inline Window& getWindow() const { return *m_window; }

	private:
		int m_status { 0 };
		float m_lastFrameTime { 0.0f };

		std::unique_ptr<Window> m_window;
		std::shared_ptr<Scene> m_scene;

		//
		std::shared_ptr<Font> m_font;
		//
	};

	// To be defined in the game
	extern Application& createApplication();

}

#endif // APPLICATION_H

// C++17 features used:
//   - std::string:view, log.h
//   - std::shared_ptr array management, renderer.h

// OpenGL 4.5 features used:
//   -

// Gameplan
// v Entrypoint
// v Logging
// v Events
// v Window
// v Settings loader (json)
// v Input polling
// v OpenGL context
// - GPUDriver (?)
// v Renderer
//   v Buffers
//   ~ Shader
//   - Schene (camera, lights, environment)
// - Texture loading
// - Model loading
// - Entity Component System
// - Serialization
// - Level format
// - Tools (Tiled?)
// - Scripting (Lua)

// - Global object access can be done in 3 ways:
// - Singleton, static, extern

// @Todo
// - Settings should contain all file paths (ex: shaders)
// - RefPtr<>
// - Rename Application::get() to Application::the() for singleton
