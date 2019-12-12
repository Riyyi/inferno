#ifndef APPLICATION_H
#define APPLICATION_H

namespace Inferno {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		inline static Application &get() { return *s_instance; }

	private:
		static Application* s_instance;
	};

	// To be defined in the game
	Application* createApplication();

}

#endif // APPLICATION_H

// @Todo
// v Application -> Singleton
// v Add assert
// - Event class
// - Event Dispatcher
//   - template
// - Implement event in Application::OnEvent(Event& e);
