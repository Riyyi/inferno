/*
 * `m_` for member variables,
 * `s_` for static variables,
 * `g_` for global variables.
 */

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "inferno/application.h"
#include "inferno/log.h"

extern Inferno::Application* Inferno::createApplication();

int main(int argc, char* argv[])
{
	// Supress unused warning
	(void)argc;
	(void)argv;

	// Initialize Log
	Inferno::Log::init();
	NF_CORE_INFO("Initialized Log!");

	// Start application
	auto app = Inferno::createApplication();
	app->run();
	delete app;

	return 0;
}

#endif // ENTRYPOINT_H
