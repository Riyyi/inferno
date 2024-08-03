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

private:
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
