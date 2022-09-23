/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "inferno.h"
#include "inferno/entrypoint.h"

class Game : public Inferno::Application
{
public:
	Game() : Application({}) {}
	~Game() {}
};

Inferno::Application& Inferno::createApplication()
{
	return Game::the();
}
