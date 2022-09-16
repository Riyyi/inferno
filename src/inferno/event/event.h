#ifndef EVENT_H
#define EVENT_H

#include <ostream> // std::ostream

#include "inferno/core.h"

namespace Inferno {

enum class EventType {
	None = 0,

	WindowClose,
	WindowResize,

	KeyPress,
	KeyRelease,
	KeyRepeat,

	MouseButtonPress,
	MouseButtonRelease,
	MousePosition,
	MouseScroll,

	JoystickConnected,
	JoystickDisconnected,
};

enum EventCategory {
	None = 0,
	ApplicationEventCategory = BIT(0),

	InputEventCategory = BIT(1),

	KeyEventCategory = BIT(2),

	MouseEventCategory = BIT(3),
	MouseButtonEventCategory = BIT(4),

	JoystickEventCatergory = BIT(5),
};

class Event {
	// EventDispatcher is allowed to access Event private/protected members
	friend class EventDispatcher;

public:
	virtual ~Event() {}

	virtual std::string toString() const { return getName(); }

	inline bool inCategory(EventCategory category)
	{
		return getCategoryFlags() & category;
	}

	// -----------------------------------------

	// Getter function templates
	virtual char getCategoryFlags() const = 0;
	virtual const char* getName() const = 0;
	virtual EventType getType() const = 0;

private:
	bool handled = false;
};

class EventDispatcher {
public:
	EventDispatcher(Event& e)
		: m_event(e)
	{
	}

	// Easily dispatch all type of Events, call with:
	// dispatch<T>(std::bind(&F, this, std::placeholders::_1));
	// T is the type of Event
	// F is the function to call, signature: bool name(T& e);
	template<typename T, typename F>
	bool dispatch(const F& function)
	{
		// If <constructed> type is same as member variable type
		if (T::getTypeStatic() == m_event.getType()) {
			// Call the function
			m_event.handled = function(static_cast<T&>(m_event));
			return true;
		}

		return false;
	}

private:
	Event& m_event;
};

// Add class type functions macro
#define EVENT_CLASS_TYPE(type)                                             \
	virtual const char* getName() const override { return #type; }         \
	virtual EventType getType() const override { return getTypeStatic(); } \
	static inline EventType getTypeStatic() { return EventType::type; }

// Add class category function macro
#define EVENT_CLASS_CATEGORY(category) \
	virtual char getCategoryFlags() const override { return category; }

// Make Events easily printable
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.toString();
}

} // namespace Inferno

#endif // EVENT_H
