#include "inferno.h"
#include "inferno/entrypoint.h"

class Game : public Inferno::Application
{
public:
	Game() {};
	~Game() {};
};

Inferno::Application* Inferno::createApplication()
{
	return new Game();
}
