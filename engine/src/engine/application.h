#ifndef APPLICATION_H
#define APPLICATION_H

namespace Engine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the game
	Application *CreateApplication();

}

#endif // APPLICATION_H
