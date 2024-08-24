/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t

#include "glad/glad.h"
#include "ruc/format/log.h"

#include "inferno/component/cubemap-component.h"
#include "inferno/component/model-component.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/render/framebuffer.h"
#include "inferno/render/render-command.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader-storage-buffer.h"
#include "inferno/render/shader-structs.h"
#include "inferno/render/uniformbuffer.h"
#include "inferno/system/camerasystem.h"
#include "inferno/system/rendersystem.h"
#include "inferno/system/textareasystem.h"

namespace Inferno {

RenderSystem::RenderSystem(s)
{
}

RenderSystem::~RenderSystem()
{
}

// -----------------------------------------

void RenderSystem::initialize(uint32_t width, uint32_t height)
{
	m_framebuffer = Framebuffer::create({
		.attachments = { Framebuffer::Type::Color, Framebuffer::Type::RGBA16F, Framebuffer::Type::RGBA16F, Framebuffer::Type::Depth },
		.width = width,
		.height = height,
		.clearColor = { 0.0f, 0.0f, 0.0f, 0.0f },
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
			{ BufferElementType::Vec3, "u_position" },
		});
	Uniformbuffer::the().create("Camera");

	ShaderStorageBuffer::the().setLayout("DirectionalLights", 0, RendererPostProcess::the().shaderID());
	ShaderStorageBuffer::the().create("DirectionalLights");

	ruc::info("RenderSystem initialized");
}

void RenderSystem::render()
{
	static constexpr TransformComponent transformIdentity;

	// ---------------------------------
	// Deferred rendering to a framebuffer

	framebufferSetup(m_framebuffer);

	renderGeometry();

	framebufferTeardown(m_framebuffer);

	// ---------------------------------
	// Forward rendering to the screen

	framebufferSetup(m_screenFramebuffer);

	renderSkybox();

	// Render 3D geometry post-processing
	RendererPostProcess::the().drawQuad(transformIdentity, m_framebuffer->texture(0), m_framebuffer->texture(1), m_framebuffer->texture(2));
	RendererPostProcess::the().endScene();

	// Visual representation of light sources
	Framebuffer::copyBuffer(m_framebuffer, m_screenFramebuffer, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	renderLightCubes();

	// Render 2D, UI
	renderOverlay();

	framebufferTeardown(m_screenFramebuffer);
}

void RenderSystem::resize(int32_t width, int32_t height)
{
	RenderCommand::setViewport(0, 0, width, height);
	m_framebuffer->resize(width, height);
	m_screenFramebuffer->resize(width, height);
}

// -----------------------------------------

void RenderSystem::framebufferSetup(std::shared_ptr<Framebuffer> framebuffer)
{
	framebuffer->bind();

	RenderCommand::setColorAttachmentCount(framebuffer->colorAttachmentCount());
	RenderCommand::clearColor(framebuffer->clearColor());
	RenderCommand::clearBit(framebuffer->clearBit());
}

void RenderSystem::framebufferTeardown(std::shared_ptr<Framebuffer> framebuffer)
{
	framebuffer->unbind();

	RenderCommand::setColorAttachmentCount(1);
}

void RenderSystem::renderGeometry()
{
	auto [projection, view] = CameraSystem::the().projectionView();
	auto translate = CameraSystem::the().translate();
	Uniformbuffer::the().setValue("Camera", "u_projectionView", projection * view);
	Uniformbuffer::the().setValue("Camera", "u_position", translate);

	static DirectionalLightBlock directionalLights[2] = {
		{
			.direction = { -8.0f, -8.0f, -8.0f },
			.ambient = { 0.1f, 0.1f, 0.1f },
			.diffuse = { 1.0f, 1.0f, 1.0f },
			.specular = { 1.0f, 1.0f, 1.0f },
		},
		{
			.direction = { 8.0f, 8.0f, 8.0f },
			.ambient = { 0.1f, 0.1f, 0.1f },
			.diffuse = { 1.0f, 1.0f, 1.0f },
			.specular = { 1.0f, 0.0f, 0.0f },
		},
	};
	ShaderStorageBuffer::the().setValue("DirectionalLights", "u_directionalLight", directionalLights);

	auto modelView = m_registry->view<TransformComponent, ModelComponent>();

	for (auto [entity, transform, model] : modelView.each()) {
		Renderer3D::the().drawModel(model.model->vertices(),
		                            model.model->elements(),
		                            transform,
		                            model.color,
		                            model.model->texture() ? model.model->texture() : model.texture);
	}

	Renderer3D::the().endScene();
}

void RenderSystem::renderSkybox()
{
	auto [projection, view] = CameraSystem::the().projectionView();
	RendererCubemap::the().beginScene(projection, view); // camera, lights, environment

	auto cubemapView = m_registry->view<TransformComponent, CubemapComponent>();

	for (auto [entity, transform, cubemap] : cubemapView.each()) {
		if (!cubemap.isLight) {
			RendererCubemap::the().drawCubemap(transform, cubemap.color, cubemap.texture);
		}
	}

	RendererCubemap::the().endScene();
}

void RenderSystem::renderLightCubes()
{
	auto [projection, view] = CameraSystem::the().projectionView();
	RendererLightCube::the().beginScene(projection, view); // camera, lights, environment

	auto cubemapView = m_registry->view<TransformComponent, CubemapComponent>();

	for (auto [entity, transform, cubemap] : cubemapView.each()) {
		if (cubemap.isLight) {
			RendererLightCube::the().drawCubemap(transform, cubemap.color, nullptr);
		}
	}

	RendererLightCube::the().endScene();
}

void RenderSystem::renderOverlay()
{
	auto quadView = m_registry->view<TransformComponent, SpriteComponent>();

	for (auto [entity, transform, sprite] : quadView.each()) {
		Renderer2D::the().drawQuad(transform, sprite.color, sprite.texture);
	}

	Renderer2D::the().endScene();

	TextAreaSystem::the().render();
	RendererFont::the().endScene();
}

} // namespace Inferno
