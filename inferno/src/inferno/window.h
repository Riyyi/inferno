#ifndef WINDOW_H
#define WINDOW_H

#include <functional> // std::function
#include <memory>     // std::shared_ptr

struct GLFWwindow;

namespace Inferno {

	class Context;
	class Event;

	struct WindowProperties {
		const char* title = "Inferno";
		int width = 1280;
		int height = 720;
		const char* fullscreen = "windowed"; // windowed/fullscreen/borderless
		bool vsync = true;
	};

	class Window {
	public:
		Window();
		virtual ~Window();

// -----------------------------------------

		void initialize();
		void update();
		void render();
		void destroy();

// -----------------------------------------

		void setWindowMonitor();
		bool shouldClose() const;
		void setShouldClose(bool close) const;

		inline float getAspect() const { return static_cast<float>(m_properties.width) / static_cast<float>(m_properties.height); }
		inline int getWidth() const { return m_properties.width; }
		inline int getHeight() const { return m_properties.height; }
		inline bool isVSync() const { return m_properties.vsync; }

		inline GLFWwindow* getWindow() const { return m_window; }
		inline const std::shared_ptr<Context>& getContext() const { return m_context; }

		inline void setEventCallback(std::function<void(Event&)> callback) { m_eventCallback = callback; }

	private:
		WindowProperties m_properties;
		GLFWwindow* m_window;
		std::shared_ptr<Context> m_context;

		std::function<void(Event&)> m_eventCallback;

		static unsigned char s_windowCount;
	};

}

#endif // WINDOW_H
