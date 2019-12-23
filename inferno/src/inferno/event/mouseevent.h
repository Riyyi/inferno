#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <sstream> // std::stringstream

#include "inferno/event/event.h"

namespace Inferno {

	class MouseButtonEvent : public Event {
	public:
		inline int getButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory | MouseButtonEventCategory)

	protected:
		MouseButtonEvent(int button) :
			m_button(button) {}

	private:
		int m_button;
	};

	class MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << getButton();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << getButton();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonRelease)
	};

	class MousePositionEvent : public Event {
	public:
		MousePositionEvent(float xPos, float yPos) :
			m_xPos(xPos), m_yPos(yPos) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MousePosition: " << m_xPos << "x" << m_yPos;
			return ss.str();
		}

// -----------------------------------------

		inline float getXPos() const { return m_xPos; }
		inline float getYPos() const { return m_yPos; }

		EVENT_CLASS_TYPE(MousePosition)
		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		float m_xPos;
		float m_yPos;
	};

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset) :
			m_xOffset(xOffset), m_yOffset(yOffset) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScroll: " << m_xOffset << ":" << m_yOffset;
			return ss.str();
		}

// -----------------------------------------

		inline float getXOffset() const { return m_xOffset; }
		inline float getYOffset() const { return m_yOffset; }

		EVENT_CLASS_TYPE(MouseScroll)
		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		float m_xOffset;
		float m_yOffset;
	};

}

#endif // MOUSEEVENT_H
