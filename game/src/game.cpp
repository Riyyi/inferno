#include "engine.h"

class Game : public Engine::Application
{
public:
	Game() {};
	~Game() {};
};

Engine::Application *Engine::createApplication()
{
	return new Game();
}
