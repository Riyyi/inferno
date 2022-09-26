/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "game.h"

Game::Game()
	: Application()
{
}

Game::~Game()
{
}

void Game::update()
{
}

void Game::render()
{
}

Inferno::Application* Inferno::createApplication(int argc, char* argv[])
{
	return new Game;
}
