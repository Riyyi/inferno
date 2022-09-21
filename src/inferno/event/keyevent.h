#pragma once

#include <sstream> // std::stringstream

#include "inferno/event/event.h"

namespace Inferno {

	class KeyEvent : public Event {
	public:
		inline int getKey() const { return m_key; }

		EVENT_CLASS_CATEGORY(InputEventCategory | KeyEventCategory)

	protected:
		KeyEvent(int key) :
			m_key(key) {}

	private:
		int m_key;
	};

	class KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int key) :
			KeyEvent(key) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPress: " << getKey();
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress)
	};

	class KeyReleaseEvent : public KeyEvent {
	public:
		KeyReleaseEvent(int key) :
			KeyEvent(key) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyRelease: " << getKey();
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress)
	};

	class KeyRepeatEvent : public KeyEvent {
	public:
		KeyRepeatEvent(int key) :
			KeyEvent(key) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyRepeat: " << getKey();
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress)
	};

} // namespace Inferno
