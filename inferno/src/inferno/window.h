#ifndef WINDOW_H
#define WINDOW_H

#include <functional> // function

class GLFWwindow;

namespace Inferno {

	class Event;

	struct WindowProperties {
		const char* title = "Inferno";
		unsigned int width = 1280;
		unsigned int height = 720;
	};

	class Window {
	public:
		Window(const WindowProperties &properties = WindowProperties());
		virtual ~Window();

		void initialize();
		void update();
		// void render();
		void destroy();

// -----------------------------------------

		inline int getWidth() const { return m_windowProperties.width; }
		inline int getHeight() const { return m_windowProperties.height; }
		inline GLFWwindow* getWindow() const { return m_window; }

		inline void setEventCallback(std::function<void(Event&)> callback) { m_eventCallback = callback; }

	private:
		WindowProperties m_windowProperties;
		GLFWwindow* m_window;

		std::function<void(Event&)> m_eventCallback;

		static unsigned char s_windowCount;
	};

}

#endif // WINDOW_H