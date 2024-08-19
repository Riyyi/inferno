/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // int32_t, uint32_t
#include <memory>  //std::shared_ptr

#include "entt/entity/fwd.hpp" // entt::registry

#include "ruc/singleton.h"

namespace Inferno {

class Framebuffer;

class RenderSystem final : public ruc::Singleton<RenderSystem> {
public:
	RenderSystem(s);
	virtual ~RenderSystem();

	void initialize(uint32_t width, uint32_t height);
	void render();

	void resize(int32_t width, int32_t height);

	void setRegistry(std::shared_ptr<entt::registry> registry) { m_registry = registry; };

private:
	void framebufferSetup(std::shared_ptr<Framebuffer> framebuffer);
	void framebufferTeardown(std::shared_ptr<Framebuffer> framebuffer);
	void renderGeometry();
	void renderSkybox();
	void renderLightCubes();
	void renderOverlay();

	std::shared_ptr<Framebuffer> m_framebuffer;
	std::shared_ptr<Framebuffer> m_screenFramebuffer;
	std::shared_ptr<entt::registry> m_registry;
};

} // namespace Inferno
