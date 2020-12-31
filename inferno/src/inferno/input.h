#ifndef INPUT_H
#define INPUT_H

#include <utility> // std::pair

namespace Inferno {

	class MousePositionEvent;

	class Input {
	public:
		static void update();

		static bool onMousePosition(MousePositionEvent& e);

		static bool isKeyPressed(int key);
		static bool isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();

// -----------------------------------------

		static inline float getXOffset() { return m_xOffset; }
		static inline float getYOffset() { return m_yOffset; }

	private:
		static bool m_firstMousePos;
		static float m_xPosLast;
		static float m_yPosLast;
		static float m_xOffset;
		static float m_yOffset;
	};

}

#endif // INPUT_H
