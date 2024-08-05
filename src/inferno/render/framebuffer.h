/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <initializer_list>
#include <memory> // std::shared_ptr
#include <vector>

#include "glm/ext/vector_float4.hpp" // glm::vec4

#include "inferno/asset/texture.h"

namespace Inferno {

class Framebuffer final { // Frame Buffer Object, FBO
public:
	enum Type : uint8_t {
		None = 0,

		// Color
		RGBA8 = 1,

		// Depth/stencil
		Depth32F = 2,
		Depth24Stencil8 = 3,

		// Defaults
		Color = RGBA8,
		Depth = Depth24Stencil8,
	};

	struct TypeProperties {
		TypeProperties() = default;
		TypeProperties(Type type)
			: type(type)
		{
		}

		Type type;
	};

	struct Properties {
		std::initializer_list<TypeProperties> attachments {};
		bool renderToScreen { false }; // (dummy framebuffer)

		uint32_t width { 1280 };
		uint32_t height { 720 };
		glm::vec4 clearColor { 1.0f, 0.0f, 1.0f, 1.0f }; // magenta
		uint32_t clearBit { 0 };
	};

	~Framebuffer();

	// Factory function
	static std::shared_ptr<Framebuffer> create(const Properties& properties);

	void bind() const;
	void unbind() const;
	bool check() const;
	void resize(uint32_t width, uint32_t height);

	uint32_t id() const { return m_id; }
	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t clearBit() const { return m_clearBit; }
	glm::vec4 clearColor() const { return m_clearColor; }
	const std::vector<TypeProperties>& attachments() const { return m_attachments; }
	std::shared_ptr<TextureFramebuffer> texture(size_t index) const { return m_textures[index]; }

private:
	Framebuffer(const Properties& properties)
		: m_renderToScreen(properties.renderToScreen)
		, m_width(properties.width)
		, m_height(properties.height)
		, m_clearBit(properties.clearBit)
		, m_clearColor(properties.clearColor)
		, m_attachments(properties.attachments)
	{
	}
	void createTextures();

private:
	bool m_renderToScreen { false };
	uint32_t m_id { 0 };
	uint32_t m_width { 0 };
	uint32_t m_height { 0 };
	uint32_t m_clearBit { 0 };
	glm::vec4 m_clearColor;
	std::vector<TypeProperties> m_attachments;
	std::vector<std::shared_ptr<TextureFramebuffer>> m_textures;
};

} // namespace Inferno
