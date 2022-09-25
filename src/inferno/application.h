/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::unique_ptr, std::shared_ptr

#include "ruc/singleton.h"

namespace Inferno {

class Event;
class Font;
class KeyPressEvent;
class MousePositionEvent;
class Scene;
class Window;
class WindowCloseEvent;
class WindowResizeEvent;

class Application : public ruc::Singleton<Application> {
public:
	explicit Application(s);
	virtual ~Application();

	int run();

	void onEvent(Event& e);
	bool onWindowClose(WindowCloseEvent& e);
	bool onWindowResize(WindowResizeEvent& e);
	bool onKeyPress(KeyPressEvent& e);
	bool onMousePosition(MousePositionEvent& e);

	inline void setStatus(int status) { m_status = status; }

	inline Window& getWindow() const { return *m_window; }

private:
	int m_status { 0 };
	float m_lastFrameTime { 0.0f };

	std::unique_ptr<Window> m_window;
	std::shared_ptr<Scene> m_scene;

	//
	std::shared_ptr<Font> m_font;
	//
};

// To be defined in the game
extern Application& createApplication();

} // namespace Inferno

// C++17 features used:
//   - std::string_view                 -> log.h
//   - std::shared_ptr array management -> renderer.h
//   - EnTT library
//   - structured binding               -> render.cpp, camera.cpp
//   - std::optional                    -> textareasystem.cpp

// OpenGL 4.5 features used:
//   -

// Cmake 3.16 features used:
//   - Precompiled headers

// Gameplan
// - GPUDriver (?)
// v Renderer
//   v Buffers
//   ~ Shader
//   - Schene (camera, lights, environment)
// - Model loading
// - Serialization
// - Level format
// - Tools (Tiled?)

// - Global object access can be done in 4 ways:
// - Singleton, static class, extern, regular global

// @Todo
// - RefPtr<>
// - keycodes.h stringify helper in code.h (util/types.h, util/defines.h)
//     https://github.com/SerenityOS/serenity/blob/master/Kernel/Assertions.h#L29
// - Call delete on pointers that make the singletons, so valgrind will say "All heap blocks were freed -- no leaks are possible", in "HEAP SUMMARY"
// - TagComponent, string -> char[]
// - Move Gltf parsing and json helper to /parser directory

// - Initialize primitive types, raw pointers are uninitialized, so need to be explicitly initialized to nullptr
// - Run clang-format on every file

// profiling: 1:33:08 - JS bytecode VM part 7
// $ valgrind --tool=callgrind <executable>
// $ kcachegrind callgrind.out.<number>

// GUI
// Framebuffer on entire screen, manage every individual pixel
// - classes:    Window, Button, Label
// - event loop: select based
