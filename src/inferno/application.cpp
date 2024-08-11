/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <utility> // std::pair

#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp" // glm::make_mat4
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/application.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/core.h"
#include "inferno/event/applicationevent.h"
#include "inferno/event/event.h"
#include "inferno/event/keyevent.h"
#include "inferno/event/mouseevent.h"
#include "inferno/io/input.h"
#include "inferno/keycodes.h"
#include "inferno/render/buffer.h"
#include "inferno/render/context.h"
#include "inferno/render/framebuffer.h"
#include "inferno/render/uniformbuffer.h"
// #include "inferno/render/gltf.h"
#include "inferno/asset/shader.h"
#include "inferno/asset/texture.h"
#include "inferno/render/render-command.h"
#include "inferno/render/renderer.h"
#include "inferno/scene/scene.h"
#include "inferno/settings.h"
#include "inferno/time.h"
#include "inferno/window.h"

namespace Inferno {

Application* Application::s_instance = nullptr;

Application::Application()
{
	// Set singleton instance
	VERIFY(!s_instance, "reinstantiation of Application");
	s_instance = this;

	// Initialize

	Settings::initialize();

	m_window = std::make_unique<Window>();
	m_window->setEventCallback(NF_BIND_EVENT(Application::onEvent));

	Input::initialize();
	RenderCommand::initialize();

	m_framebuffer = Framebuffer::create({
		.attachments = { Framebuffer::Type::Color, Framebuffer::Type::Depth },
		.width = m_window->getWidth(),
		.height = m_window->getHeight(),
		.clearColor = { 0.2f, 0.3f, 0.3f, 1.0f },
		.clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
	});

	m_screenFramebuffer = Framebuffer::create({
		.renderToScreen = true,
		.clearColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		.clearBit = GL_COLOR_BUFFER_BIT,
	});

	Uniformbuffer::the().setLayout(
		"Camera", 0,
		{
			{ BufferElementType::Mat4, "u_projectionView" },
		});
	Uniformbuffer::the().create("Camera");

	m_scene = std::make_shared<Scene>();
	m_scene->initialize();

	// m_font = FontManager::the().load("assets/fnt/dejavu-sans");

	// auto bla = GlTFFile::read("assets/gltf/box.glb");
	// ruc::success("@{}@", bla.first.get());
	// auto bla2 = GlTFFile::read("assets/gltf/boxtextured.glb");
	// ruc::info("@{}@", bla2.first.get());
	// auto bla3 = GlTFFile::read("assets/gltf/guinea-pig-cage-fleece.glb");
	// ruc::warn("@{}@", bla3.first.get());

	// Gltf model = Gltf("assets/gltf/box.glb");

	// Gltf model = Gltf("assets/gltf/animatedmorphcube.glb");
	// Gltf model = Gltf("assets/gltf/reciprocatingsaw.glb");

	// Gltf model = Gltf("assets/gltf/triangle-without-indices.gltf");
}

Application::~Application()
{
	m_scene->destroy();
	Uniformbuffer::destroy();

	RendererFont::destroy();
	Renderer2D::destroy();
	Renderer3D::destroy();
	RendererCubemap::destroy();
	RenderCommand::destroy();
	AssetManager::destroy();
	// Input::destroy();

	Settings::destroy();

	s_instance = nullptr;
}

int Application::run()
{
	ruc::debug("Application startup");

#if 0
	std::array<CharacterVertex, Renderer::vertexPerQuad> character;

	// character.at(0).quad.textureCoordinates = { 0.0f, 0.0f }; // bottom left
	// character.at(1).quad.textureCoordinates = { 1.0f, 0.0f };
	// character.at(2).quad.textureCoordinates = { 1.0f, 1.0f }; // top right
	// character.at(3).quad.textureCoordinates = { 0.0f, 1.0f };

	auto f = FontManager::the().get("assets/fnt/dejavu-sans");
	auto c = f->get('5');
	// ruc::debug(c->position << " " << c->size);

	uint32_t textureWidth = f->texture()->width();
	uint32_t textureHeight = f->texture()->height();
	VERIFY(textureWidth == textureHeight, "Invalid font texture!");

	float quadWidth = (c->size.x / (float)textureWidth) - 0.04; // @Todo something wrong with the width
	float quadHeight = c->size.y / (float)textureHeight;

	character.at(0).quad.position = { -quadWidth, -quadHeight, 0.0f }; // bottom left
	character.at(1).quad.position = { quadWidth, -quadHeight, 0.0f };  // bottom right
	character.at(2).quad.position = { quadWidth, quadHeight, 0.0f };   // top right
	character.at(3).quad.position = { -quadWidth, quadHeight, 0.0f };  // top left

	glm::vec2 x {
		1 - (textureWidth - c->position.x) / (float)textureWidth,
		1 - (textureWidth - c->position.x - c->size.x) / (float)textureWidth
	};
	glm::vec2 y {
		(textureHeight - c->position.y - c->size.y) / (float)textureHeight,
		(textureHeight - c->position.y) / (float)textureHeight
	};
	// ruc::debug(y);

	character.at(0).quad.textureCoordinates = { x.x, y.x };
	character.at(1).quad.textureCoordinates = { x.y, y.x };
	character.at(2).quad.textureCoordinates = { x.y, y.y };
	character.at(3).quad.textureCoordinates = { x.x, y.y };

	// pos
	// texcoords
	//
	// width
	// edge
	// borderwidth
	// borderedge
	// bordercolor
	// offset
#endif

	constexpr glm::vec4 vectorOne { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr glm::mat4 matIdentity { 1.0f };
	constexpr TransformComponent transformIdentity;

	double gametime = 0;
	uint64_t frames = 0;

	while (!m_window->shouldClose()) {

		float time = Time::time();
		float deltaTime = time - m_lastFrameTime;
		m_lastFrameTime = time;
		// ruc::debug("Frametime {}ms", deltaTime * 1000);

		gametime += deltaTime;
		frames++;

		// ---------------------------------
		// Update

		update();

		Input::update();
		m_window->update();
		m_scene->update(deltaTime);

		// ---------------------------------
		// Render

		m_framebuffer->bind();

		RenderCommand::clearColor(m_framebuffer->clearColor());
		RenderCommand::clearBit(m_framebuffer->clearBit());

		render();

		m_scene->render();
		// RendererCharacter::the().drawCharacter(character, f->texture());

		m_framebuffer->unbind();

		// ---------------------------------
		// Framebuffer

		m_screenFramebuffer->bind();

		RenderCommand::clearColor(m_screenFramebuffer->clearColor());
		RenderCommand::clearBit(m_screenFramebuffer->clearBit());

		Renderer2D::the().setEnableDepthBuffer(false);
		Uniformbuffer::the().setValue("Camera", "u_projectionView", matIdentity);
		Renderer2D::the().drawQuad(transformIdentity, vectorOne, m_framebuffer->texture(0));
		Renderer2D::the().endScene();
		Renderer2D::the().setEnableDepthBuffer(true);

		m_screenFramebuffer->unbind();

		m_window->render();
	}

	ruc::debug("Application shutdown");

	ruc::debug("Average frametime: {:.2f}ms", (gametime / frames) * 1000);

	return m_status;
}

void Application::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>(NF_BIND_EVENT(Application::onWindowClose));
	dispatcher.dispatch<WindowResizeEvent>(NF_BIND_EVENT(Application::onWindowResize));
	dispatcher.dispatch<KeyPressEvent>(NF_BIND_EVENT(Application::onKeyPress));
	dispatcher.dispatch<MousePositionEvent>(NF_BIND_EVENT(Application::onMousePosition));
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
	// Suppress unused warning
	(void)e;

	ruc::info("WindowCloseEvent");

	m_window->setShouldClose(true);

	return true;
}

bool Application::onWindowResize(WindowResizeEvent& e)
{
	ruc::info("WindowResizeEvent {}x{}", e.getWidth(), e.getHeight());

	RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
	m_framebuffer->resize(e.getWidth(), e.getHeight());

	return true;
}

bool Application::onKeyPress(KeyPressEvent& e)
{
	ruc::info("KeyPressEvent {:3}  {}", e.getKey(), keyName(e.getKey()));

	// Stop the main loop on 'Escape' keypress
	if (e.getKey() == keyCode("GLFW_KEY_ESCAPE")) {
		m_window->setShouldClose(true);
	}

	return true;
}

bool Application::onMousePosition(MousePositionEvent& e)
{
	// ruc::info("MousePositionEvent {:.0}x{:.0}", e.getXPos(), e.getYPos());

	return Input::onMousePosition(e);
}

} // namespace Inferno
