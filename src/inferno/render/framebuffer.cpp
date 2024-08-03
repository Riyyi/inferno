/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int8_t

#include "glad/glad.h"
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/asset/texture.h"
#include "inferno/render/framebuffer.h"

namespace Inferno {

Framebuffer::Framebuffer(const Properties& init)
{
	VERIFY(static_cast<int8_t>(init.type) != 0,
	       "malformed framebuffer type: {}", init.type);

	m_id = UINT_MAX;
	glGenFramebuffers(1, &m_id);
	bind();

	if (init.type & Type::Color) {
		ruc::error("color!");
		// Set color attachment 0 out of 32
		m_textures[0] = TextureFramebuffer::create(init.width, init.height, GL_RGB, GL_RGB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[0]->id(), 0);
	}

	// This combined texture is required for older GPUs
	if (init.type & Type::Depth && init.type & Stencil) {
		ruc::error("both!");
		m_textures[3] = TextureFramebuffer::create(init.width, init.height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_textures[3]->id(), 0);
	}
	else if (init.type & Type::Depth) {
		ruc::error("depth!");
		m_textures[1] = TextureFramebuffer::create(init.width, init.height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[1]->id(), 0);
	}
	else if (init.type & Type::Stencil) {
		ruc::error("stencil!");
		m_textures[2] = TextureFramebuffer::create(init.width, init.height, GL_STENCIL_INDEX, GL_STENCIL_INDEX);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_textures[2]->id(), 0);
	}

	check();
	unbind();
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

} // namespace Inferno
