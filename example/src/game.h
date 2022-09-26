/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "inferno.h"
#include "inferno/entrypoint.h"

class Game final : public Inferno::Application {
public:
	Game();
	~Game();

	void update() override;
	void render() override;
};

Inferno::Application* Inferno::createApplication(int argc, char* argv[]);
