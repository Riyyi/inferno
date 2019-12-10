#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "application.h"

extern Engine::Application *Engine::CreateApplication();

int main(int argc, char *argv[])
{
	// Supress unused warning
	(void)argc;
	(void)argv;

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif // ENTRYPOINT_H
