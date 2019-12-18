#ifndef INPUT_H
#define INPUT_H

#include <utility>

namespace Inferno {

	class Input {
	public:
		static bool isKeyPressed(int key);
		static bool isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};

}


#endif // INPUT_H
