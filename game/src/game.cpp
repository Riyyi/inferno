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
	Game::initialize();
	return Game::the();
}
