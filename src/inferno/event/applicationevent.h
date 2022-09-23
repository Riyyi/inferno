/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <sstream> // std::stringstream

#include "inferno/event/event.h"

namespace Inferno {

class WindowCloseEvent : public Event {
public:
	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(ApplicationEventCategory)
};

class WindowResizeEvent : public Event {
public:
	WindowResizeEvent(int width, int height)
		: m_width(width)
		, m_height(height)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "WindowResize: " << m_width << "x" << m_height;
		return ss.str();
	}

	// -----------------------------------------

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(ApplicationEventCategory)

private:
	int m_width;
	int m_height;
};
} // namespace Inferno
