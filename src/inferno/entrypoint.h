/*
 * `m_` for member variables,
 * `s_` for static variables,
 * `g_` for global variables.
 */

#pragma once

#include "inferno/application.h"

int main(int argc, char* argv[])
{
	// Suppress unused warning
	(void)argc;
	(void)argv;

	auto& app = Inferno::createApplication();

	int status = app.run();

	app.destroy();

	return status;
}
