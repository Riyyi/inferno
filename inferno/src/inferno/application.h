#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory> // std::unique_ptr

namespace Inferno {

	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class Window;

	class VertexArray;
	class Shader;

	class Application {
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event &e);
		bool onWindowClose(WindowCloseEvent &e);
		bool onWindowResize(WindowResizeEvent &e);

// -----------------------------------------

		inline Window &getWindow() { return *m_window; }

		static inline Application &get() { return *s_instance; }

	private:
		std::unique_ptr<Window> m_window;

		//
		std::shared_ptr<VertexArray> m_vertexArray;
		std::unique_ptr<Shader> m_shader;
		//

		static Application* s_instance;
	};

	// To be defined in the game
	extern Application* createApplication();

}

#endif // APPLICATION_H

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
