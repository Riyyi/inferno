/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // std::find_if
#include <string_view>
#include <unordered_map> // std::unordered_map

#include "GLFW/glfw3.h"
#include "ruc/meta/assert.h"

#include "inferno/keycodes.h"

namespace Inferno {

static std::unordered_map<std::string_view, int> keys({
	{ MAP_KEY(GLFW_KEY_UNKNOWN) },
	{ MAP_KEY(GLFW_KEY_SPACE) },
	{ MAP_KEY(GLFW_KEY_APOSTROPHE) },
	{ MAP_KEY(GLFW_KEY_COMMA) },
	{ MAP_KEY(GLFW_KEY_MINUS) },
	{ MAP_KEY(GLFW_KEY_PERIOD) },
	{ MAP_KEY(GLFW_KEY_SLASH) },
	{ MAP_KEY(GLFW_KEY_0) },
	{ MAP_KEY(GLFW_KEY_1) },
	{ MAP_KEY(GLFW_KEY_2) },
	{ MAP_KEY(GLFW_KEY_3) },
	{ MAP_KEY(GLFW_KEY_4) },
	{ MAP_KEY(GLFW_KEY_5) },
	{ MAP_KEY(GLFW_KEY_6) },
	{ MAP_KEY(GLFW_KEY_7) },
	{ MAP_KEY(GLFW_KEY_8) },
	{ MAP_KEY(GLFW_KEY_9) },
	{ MAP_KEY(GLFW_KEY_SEMICOLON) },
	{ MAP_KEY(GLFW_KEY_EQUAL) },
	{ MAP_KEY(GLFW_KEY_A) },
	{ MAP_KEY(GLFW_KEY_B) },
	{ MAP_KEY(GLFW_KEY_C) },
	{ MAP_KEY(GLFW_KEY_D) },
	{ MAP_KEY(GLFW_KEY_E) },
	{ MAP_KEY(GLFW_KEY_F) },
	{ MAP_KEY(GLFW_KEY_G) },
	{ MAP_KEY(GLFW_KEY_H) },
	{ MAP_KEY(GLFW_KEY_I) },
	{ MAP_KEY(GLFW_KEY_J) },
	{ MAP_KEY(GLFW_KEY_K) },
	{ MAP_KEY(GLFW_KEY_L) },
	{ MAP_KEY(GLFW_KEY_M) },
	{ MAP_KEY(GLFW_KEY_N) },
	{ MAP_KEY(GLFW_KEY_O) },
	{ MAP_KEY(GLFW_KEY_P) },
	{ MAP_KEY(GLFW_KEY_Q) },
	{ MAP_KEY(GLFW_KEY_R) },
	{ MAP_KEY(GLFW_KEY_S) },
	{ MAP_KEY(GLFW_KEY_T) },
	{ MAP_KEY(GLFW_KEY_U) },
	{ MAP_KEY(GLFW_KEY_V) },
	{ MAP_KEY(GLFW_KEY_W) },
	{ MAP_KEY(GLFW_KEY_X) },
	{ MAP_KEY(GLFW_KEY_Y) },
	{ MAP_KEY(GLFW_KEY_Z) },
	{ MAP_KEY(GLFW_KEY_LEFT_BRACKET) },
	{ MAP_KEY(GLFW_KEY_BACKSLASH) },
	{ MAP_KEY(GLFW_KEY_RIGHT_BRACKET) },
	{ MAP_KEY(GLFW_KEY_GRAVE_ACCENT) },
	{ MAP_KEY(GLFW_KEY_WORLD_1) },
	{ MAP_KEY(GLFW_KEY_WORLD_2) },
	{ MAP_KEY(GLFW_KEY_ESCAPE) },
	{ MAP_KEY(GLFW_KEY_ENTER) },
	{ MAP_KEY(GLFW_KEY_TAB) },
	{ MAP_KEY(GLFW_KEY_BACKSPACE) },
	{ MAP_KEY(GLFW_KEY_INSERT) },
	{ MAP_KEY(GLFW_KEY_DELETE) },
	{ MAP_KEY(GLFW_KEY_RIGHT) },
	{ MAP_KEY(GLFW_KEY_LEFT) },
	{ MAP_KEY(GLFW_KEY_DOWN) },
	{ MAP_KEY(GLFW_KEY_UP) },
	{ MAP_KEY(GLFW_KEY_PAGE_UP) },
	{ MAP_KEY(GLFW_KEY_PAGE_DOWN) },
	{ MAP_KEY(GLFW_KEY_HOME) },
	{ MAP_KEY(GLFW_KEY_END) },
	{ MAP_KEY(GLFW_KEY_CAPS_LOCK) },
	{ MAP_KEY(GLFW_KEY_SCROLL_LOCK) },
	{ MAP_KEY(GLFW_KEY_NUM_LOCK) },
	{ MAP_KEY(GLFW_KEY_PRINT_SCREEN) },
	{ MAP_KEY(GLFW_KEY_PAUSE) },
	{ MAP_KEY(GLFW_KEY_F1) },
	{ MAP_KEY(GLFW_KEY_F2) },
	{ MAP_KEY(GLFW_KEY_F3) },
	{ MAP_KEY(GLFW_KEY_F4) },
	{ MAP_KEY(GLFW_KEY_F5) },
	{ MAP_KEY(GLFW_KEY_F6) },
	{ MAP_KEY(GLFW_KEY_F7) },
	{ MAP_KEY(GLFW_KEY_F8) },
	{ MAP_KEY(GLFW_KEY_F9) },
	{ MAP_KEY(GLFW_KEY_F10) },
	{ MAP_KEY(GLFW_KEY_F11) },
	{ MAP_KEY(GLFW_KEY_F12) },
	{ MAP_KEY(GLFW_KEY_F13) },
	{ MAP_KEY(GLFW_KEY_F14) },
	{ MAP_KEY(GLFW_KEY_F15) },
	{ MAP_KEY(GLFW_KEY_F16) },
	{ MAP_KEY(GLFW_KEY_F17) },
	{ MAP_KEY(GLFW_KEY_F18) },
	{ MAP_KEY(GLFW_KEY_F19) },
	{ MAP_KEY(GLFW_KEY_F20) },
	{ MAP_KEY(GLFW_KEY_F21) },
	{ MAP_KEY(GLFW_KEY_F22) },
	{ MAP_KEY(GLFW_KEY_F23) },
	{ MAP_KEY(GLFW_KEY_F24) },
	{ MAP_KEY(GLFW_KEY_F25) },
	{ MAP_KEY(GLFW_KEY_KP_0) },
	{ MAP_KEY(GLFW_KEY_KP_1) },
	{ MAP_KEY(GLFW_KEY_KP_2) },
	{ MAP_KEY(GLFW_KEY_KP_3) },
	{ MAP_KEY(GLFW_KEY_KP_4) },
	{ MAP_KEY(GLFW_KEY_KP_5) },
	{ MAP_KEY(GLFW_KEY_KP_6) },
	{ MAP_KEY(GLFW_KEY_KP_7) },
	{ MAP_KEY(GLFW_KEY_KP_8) },
	{ MAP_KEY(GLFW_KEY_KP_9) },
	{ MAP_KEY(GLFW_KEY_KP_DECIMAL) },
	{ MAP_KEY(GLFW_KEY_KP_DIVIDE) },
	{ MAP_KEY(GLFW_KEY_KP_MULTIPLY) },
	{ MAP_KEY(GLFW_KEY_KP_SUBTRACT) },
	{ MAP_KEY(GLFW_KEY_KP_ADD) },
	{ MAP_KEY(GLFW_KEY_KP_ENTER) },
	{ MAP_KEY(GLFW_KEY_KP_EQUAL) },
	{ MAP_KEY(GLFW_KEY_LEFT_SHIFT) },
	{ MAP_KEY(GLFW_KEY_LEFT_CONTROL) },
	{ MAP_KEY(GLFW_KEY_LEFT_ALT) },
	{ MAP_KEY(GLFW_KEY_LEFT_SUPER) },
	{ MAP_KEY(GLFW_KEY_RIGHT_SHIFT) },
	{ MAP_KEY(GLFW_KEY_RIGHT_CONTROL) },
	{ MAP_KEY(GLFW_KEY_RIGHT_ALT) },
	{ MAP_KEY(GLFW_KEY_RIGHT_SUPER) },
	{ MAP_KEY(GLFW_KEY_MENU) },
});

static std::unordered_map<std::string_view, int> modifiers({
	{ MAP_KEY(GLFW_MOD_SHIFT) },
	{ MAP_KEY(GLFW_MOD_CONTROL) },
	{ MAP_KEY(GLFW_MOD_ALT) },
	{ MAP_KEY(GLFW_MOD_SUPER) },
	{ MAP_KEY(GLFW_MOD_CAPS_LOCK) }, // State, not really a modifier
	{ MAP_KEY(GLFW_MOD_NUM_LOCK) },  // State, not really a modifier
});

// -----------------------------------------

// Example usage:
//   event.getKey() == keyCode("GLFW_KEY_ESCAPE")
int keyCode(std::string_view name)
{
	VERIFY(keys.find(name) != keys.end(), "could not find key code: {}", name);
	return keys.at(name);
}

// Example usage:
//   event.getMods() & keyMod("GLFW_MOD_SHIFT")
int keyMod(std::string_view name)
{
	VERIFY(modifiers.find(name) != modifiers.end(), "could not find key modifier: {}", name);
	return modifiers.at(name);
}

std::string_view keyName(int key)
{
	auto it = std::find_if(keys.begin(), keys.end(), [key](const auto& keybind) {
		return keybind.second == key;
	});

	VERIFY(it != keys.end(), "could not find key name: {}", key);
	return it->first.substr(9);
}

} // namespace Inferno
