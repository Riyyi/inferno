#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory> // unique_ptr

namespace Inferno {

	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class Window;

	class Application {
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event &e);
		bool onWindowClose(WindowCloseEvent &e);
		bool onWindowResize(WindowResizeEvent &e);

// -----------------------------------------

		static inline Application &get() { return *s_instance; }

	private:
		std::unique_ptr<Window> m_window;

		static Application* s_instance;
	};

	// To be defined in the game
	extern Application* createApplication();

}

#endif // APPLICATION_H
