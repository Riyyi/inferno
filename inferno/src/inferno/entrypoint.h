/*
 * `m_` for member variables,
 * `s_` for static variables,
 * `g_` for global variables.
 */

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "inferno/application.h"

int main(int argc, char* argv[])
{
	// Suppress unused warning
	(void)argc;
	(void)argv;

	// Start application
	auto app = Inferno::createApplication();
	app->run();
	delete app;

	return 0;
}

#endif // ENTRYPOINT_H
