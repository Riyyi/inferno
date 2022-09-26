/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * `m_` for member variables,
 * `s_` for static variables,
 * `g_` for global variables.
 */

#pragma once

#include "inferno/application.h"

int main(int argc, char* argv[]) // NOLINT(misc-definitions-in-headers)
{
	auto* app = Inferno::createApplication(argc, argv);

	int status = app->run();

	delete app;

	return status;
}
