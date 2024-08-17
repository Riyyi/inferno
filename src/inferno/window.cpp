/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <csignal> // signal

#include "GLFW/glfw3.h"
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/application.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/io/input.h"
#include "inferno/keycodes.h"
#include "inferno/render/context.h"
#include "inferno/render/render-command.h"
#include "inferno/render/renderer.h"
#include "inferno/settings.h"
#include "inferno/window.h"

namespace Inferno {

unsigned char Window::s_windowCount = 0;

Window::Window()
{
	m_properties = {
		Settings::get().window.title,
		Settings::get().window.width,
		Settings::get().window.height,
		Settings::get().window.fullscreen,
		Settings::get().window.vsync,
	};

	this->initialize();
}

Window::~Window()
{
	this->destroy();
}

// -----------------------------------------

void Window::initialize()
{
	std::string title = m_properties.title;
	uint32_t width = m_properties.width;
	uint32_t height = m_properties.height;
	bool vsync = m_properties.vsync;

	// Only init once
	if (s_windowCount == 0) {
		glfwInit();
	}

	// Set window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create GLFW window
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	s_windowCount++;
	VERIFY(m_window, "Failed to create GLFW window!");

	// Set windowed/fullscreen/borderless
	this->setWindowMonitor();

	// Associate the wrapper to the window
	glfwSetWindowUserPointer(m_window, this);

	// Create graphics context
	m_context = std::make_shared<Context>(m_window);
	m_context->initialize();

	// Set vsync, viewport
	setVSync(vsync);
	RenderCommand::setViewport(0, 0, width, height);

	// Signal callbacks
	signal(SIGINT, Window::signalCallback);
	signal(SIGTERM, Window::signalCallback);

	// Error callback
	glfwSetErrorCallback([](int error, const char* description) {
		ruc::error("GLFW Error: {}: {}", error, description);
	});

	// Window close callback
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		w.m_eventCallback(event);
	});

	// Window resize callback
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		w.m_properties.width = width;
		w.m_properties.height = height;

		WindowResizeEvent event(width, height);
		w.m_eventCallback(event);
	});

	// Keyboard callback
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
		// Suppress unused warning
		(void)scanCode;
		(void)mods;

		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		switch (action) {
		case GLFW_PRESS: {
			KeyPressEvent event(key, mods);
			w.m_eventCallback(event);
			break;
		}
		case GLFW_RELEASE: {
			KeyReleaseEvent event(key, mods);
			w.m_eventCallback(event);
			break;
		}
		case GLFW_REPEAT: {
			KeyRepeatEvent event(key, mods);
			w.m_eventCallback(event);
			break;
		}
		}
	});

	// Mouse button callback
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		// Suppress unused warning
		(void)mods;

		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		switch (action) {
		case GLFW_PRESS: {
			MouseButtonPressEvent event(button);
			w.m_eventCallback(event);
			break;
		}
		case GLFW_RELEASE: {
			MouseButtonReleaseEvent event(button);
			w.m_eventCallback(event);
			break;
		}
		}
	});

	// Mouse position callback
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		MousePositionEvent event(xPos, yPos);
		w.m_eventCallback(event);
	});

	// Mouse scroll callback
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Window& w = *(Window*)glfwGetWindowUserPointer(window);

		MouseScrollEvent event(xOffset, yOffset);
		w.m_eventCallback(event);
	});
}

void Window::destroy()
{
	m_context->destroy();

	glfwDestroyWindow(m_window);
	s_windowCount--;

	if (s_windowCount == 0) {
		glfwTerminate();
	}
}

void Window::update()
{
	glfwPollEvents();

	// Capture cursor in window and hide it
	if (!Input::isKeyPressed(keyCode("GLFW_KEY_LEFT_SUPER"))) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::render()
{
	m_context->render();
}

// -----------------------------------------

void Window::signalCallback(int signal)
{
	Application::the().setStatus(signal);

	if (signal == SIGINT || signal == SIGTERM) {
		WindowCloseEvent e;
		Application::the().window().m_eventCallback(e);
	}
}

void Window::setWindowMonitor() const
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int xPos = 0;
	int yPos = 0;
	unsigned int width = m_properties.width;
	unsigned int height = m_properties.height;
	int refresh = GLFW_DONT_CARE;

	std::string fullscreen = m_properties.fullscreen;
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (fullscreen.compare("fullscreen") == 0) {
		refresh = mode->refreshRate;
	}
	else if (fullscreen.compare("borderless") == 0) {
		width = mode->width;
		height = mode->height;
		refresh = mode->refreshRate;
	}
	// Default window state is windowed
	else {
		monitor = nullptr;
		// Put window in the center of the monitor
		xPos = (mode->width - width) / 2;
		yPos = (mode->height - height) / 2;
	}

	glfwSetWindowMonitor(m_window, monitor, xPos, yPos, width, height, refresh);
}

void Window::setVSync(bool enabled)
{
	enabled ? glfwSwapInterval(GL_TRUE) : glfwSwapInterval(GL_FALSE);
	m_properties.vsync = enabled;
}

void Window::setShouldClose(bool close) const
{
	glfwSetWindowShouldClose(m_window, close ? GL_TRUE : GL_FALSE);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

} // namespace Inferno
