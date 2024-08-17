/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <sstream> // std::stringstream

#include "inferno/event/event.h"

namespace Inferno {

class KeyEvent : public Event {
public:
	int getKey() const { return m_key; }
	int getMods() const { return m_mods; }

	EVENT_CLASS_CATEGORY(InputEventCategory | KeyEventCategory)

protected:
	KeyEvent(int key, int mods)
		: m_key(key)
		, m_mods(mods)
	{
	}

private:
	int m_key { 0 };
	int m_mods { 0 };
};

class KeyPressEvent final : public KeyEvent {
public:
	KeyPressEvent(int key, int mods)
		: KeyEvent(key, mods)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyPress: " << getKey();
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPress)
};

class KeyReleaseEvent final : public KeyEvent {
public:
	KeyReleaseEvent(int key, int mods)
		: KeyEvent(key, mods)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyRelease: " << getKey();
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyRelease)
};

class KeyRepeatEvent final : public KeyEvent {
public:
	KeyRepeatEvent(int key, int mods)
		: KeyEvent(key, mods)
	{
	}

	virtual std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyRepeat: " << getKey();
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyRepeat)
};

} // namespace Inferno
