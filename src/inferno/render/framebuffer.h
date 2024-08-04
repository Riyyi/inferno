/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // int8_t
#include <memory>  // std::shared_ptr

#include "inferno/asset/texture.h"
#include "ruc/meta/core.h"

// AssetManager paths to the global framebuffer textures
#define FRAMEBUFFER_TEXTURE_COLOR "@framebuffer-color"
#define FRAMEBUFFER_TEXTURE_DEPTH "@framebuffer-depth"
#define FRAMEBUFFER_TEXTURE_STENCIL "@framebuffer-stencil"
#define FRAMEBUFFER_TEXTURE_DEPTH_STENCIL "@framebuffer-depth-stencil"

namespace Inferno {

class Framebuffer final { // Frame Buffer Object, FBO
public:
	enum Type : int8_t {
		None = 0,
		Color = BIT(0),
		Depth = BIT(1),
		Stencil = BIT(2),
	};

	struct Properties {
		Type type { None };
		uint32_t width { 1280 };
		uint32_t height { 720 };
	};

	Framebuffer(const Properties& init);
	~Framebuffer();

	bool check() const;

	void bind() const;
	void unbind() const;

	void setSize(uint32_t width, uint32_t height);

	Type type() const { return m_type; }
	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t id() const { return m_id; }
	std::shared_ptr<TextureFramebuffer> texture(uint8_t index) const { return m_textures[index]; }

private:
	void createTextures();

private:
	Type m_type { None };
	uint32_t m_width { 0 };
	uint32_t m_height { 0 };
	uint32_t m_id { 0 };
	std::shared_ptr<TextureFramebuffer> m_textures[4] { nullptr, nullptr, nullptr, nullptr };
};

// Make bitwise OR '|' return the enum type instead of int
inline Framebuffer::Type operator|(Framebuffer::Type lhs, Framebuffer::Type rhs)
{
	return static_cast<Framebuffer::Type>(
		static_cast<int8_t>(lhs) | static_cast<int8_t>(rhs));
}

} // namespace Inferno
