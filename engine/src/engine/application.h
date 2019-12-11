#ifndef APPLICATION_H
#define APPLICATION_H

namespace Engine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in the game
	Application *createApplication();

}

#endif // APPLICATION_H
