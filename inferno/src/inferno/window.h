#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>    // uint32_t
#include <functional> // std::function
#include <memory>     // std::shared_ptr
#include <string>     // std::string

struct GLFWwindow;

namespace Inferno {

	class Context;
	class Event;

	struct WindowProperties {
		std::string title { "Inferno" };
		uint32_t width = 1280;
		uint32_t height = 720;
		std::string fullscreen { "windowed" }; // windowed/fullscreen/borderless
		bool vsync = true;
	};

	class Window {
	public:
		Window();
		virtual ~Window();

// -----------------------------------------

		void initialize();
		void destroy();
		void update();
		void render();

// -----------------------------------------

		static void signalCallback(int signal);

		void setWindowMonitor() const;
		void setVSync(bool enabled);
		void setShouldClose(bool close) const;

		bool shouldClose() const;
		inline float getAspect() const { return static_cast<float>(m_properties.width) / static_cast<float>(m_properties.height); }
		inline uint32_t getWidth() const { return m_properties.width; }
		inline uint32_t getHeight() const { return m_properties.height; }

		inline GLFWwindow* getWindow() const { return m_window; }
		inline const std::shared_ptr<Context>& getContext() const { return m_context; }

		inline void setEventCallback(const std::function<void(Event&)>& callback) { m_eventCallback = callback; }

	private:
		WindowProperties m_properties;
		GLFWwindow* m_window;
		std::shared_ptr<Context> m_context;

		std::function<void(Event&)> m_eventCallback;

		static unsigned char s_windowCount;
	};

}

#endif // WINDOW_H
