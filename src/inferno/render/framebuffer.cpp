/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int8_t

#include "glad/glad.h"
#include "ruc/meta/assert.h"

#include "inferno/asset/texture.h"
#include "inferno/render/framebuffer.h"

namespace Inferno {

Framebuffer::Framebuffer(const Properties& init)
	: m_type(init.type)
	, m_width(init.width)
	, m_height(init.height)
{
	VERIFY(static_cast<int8_t>(init.type) != 0,
	       "malformed framebuffer type: {}", init.type);

	m_id = UINT_MAX;
	glGenFramebuffers(1, &m_id);

	createTextures();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_id);
}

// -----------------------------------------

bool Framebuffer::check() const
{
	VERIFY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
	       "malformed framebuffer: {:#x}", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	return true;
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::setSize(uint32_t width, uint32_t height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width = width;
	m_height = height;

	createTextures();
}

// -----------------------------------------

void Framebuffer::createTextures()
{
	bind();

	m_textures[0] = AssetManager::the().remove(FRAMEBUFFER_TEXTURE_COLOR);
	m_textures[1] = AssetManager::the().remove(FRAMEBUFFER_TEXTURE_DEPTH);
	m_textures[2] = AssetManager::the().remove(FRAMEBUFFER_TEXTURE_STENCIL);
	m_textures[3] = AssetManager::the().remove(FRAMEBUFFER_TEXTURE_DEPTH_STENCIL);

	if (m_type & Type::Color) {
		// Set color attachment 0 out of 32
		m_textures[0] = AssetManager::the().load<TextureFramebuffer>(
			FRAMEBUFFER_TEXTURE_COLOR, m_width, m_height, GL_RGB, GL_RGB);
		m_textures[0]->bind(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[0]->id(), 0);
		m_textures[0]->unbind();
	}

	// This combined texture is required for older GPUs
	if (m_type & Type::Depth && m_type & Stencil) {
		m_textures[3] = AssetManager::the().load<TextureFramebuffer>(
			FRAMEBUFFER_TEXTURE_DEPTH_STENCIL, m_width, m_height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_textures[3]->id(), 0);
	}
	else if (m_type & Type::Depth) {
		m_textures[1] = AssetManager::the().load<TextureFramebuffer>(
			FRAMEBUFFER_TEXTURE_DEPTH, m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[1]->id(), 0);
	}
	else if (m_type & Type::Stencil) {
		m_textures[2] = AssetManager::the().load<TextureFramebuffer>(
			FRAMEBUFFER_TEXTURE_STENCIL, m_width, m_height, GL_STENCIL_INDEX, GL_STENCIL_INDEX);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_textures[2]->id(), 0);
	}

	check();
	unbind();
}

} // namespace Inferno
