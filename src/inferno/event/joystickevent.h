#pragma once

#include <sstream> // std::stringstream

#include "inferno/event/event.h"

namespace Inferno {

class JoystickEvent : public Event {
public:
	inline int getID() const { return m_id; }

	EVENT_CLASS_CATEGORY(InputEventCategory | JoystickEventCatergory)

protected:
	JoystickEvent(int id)
		: m_id(id)
	{
	}

private:
	int m_id;
};

class JoystickConnectedEvent : public JoystickEvent {
public:
	JoystickConnectedEvent(int id)
		: JoystickEvent(id)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "JoystickConnected: " << getID();
		return ss.str();
	}

	EVENT_CLASS_TYPE(JoystickConnected)
};

class JoystickDisconnectedEvent : public JoystickEvent {
public:
	JoystickDisconnectedEvent(int id)
		: JoystickEvent(id)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "JoystickDisconnected: " << getID();
		return ss.str();
	}

	EVENT_CLASS_TYPE(JoystickDisconnected)
};

} // namespace Inferno
