/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstddef> // size_t
#include <cstdint> // uint8_t, uint32_t

#include "glad/glad.h"
#include "ruc/meta/assert.h"

#include "inferno/asset/texture.h"
#include "inferno/render/framebuffer.h"

namespace Inferno {

std::shared_ptr<Framebuffer> Framebuffer::create(const Properties& properties)
{
	VERIFY((properties.attachments.size() > 0 && !properties.renderToScreen) || properties.renderToScreen,
	       "cant have attachments on the default framebuffer");

	auto result = std::shared_ptr<Framebuffer>(new Framebuffer(properties));

	result->createTextures();

	return result;
}

Framebuffer::~Framebuffer()
{
	if (m_renderToScreen) {
		return;
	}

	glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::copyBuffer(std::shared_ptr<Framebuffer> from, std::shared_ptr<Framebuffer> to, uint32_t bits, uint32_t filter)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, from->m_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, from->m_width, from->m_height, 0, 0, to->m_width, to->m_height, bits, filter);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// -----------------------------------------

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::check() const
{
	VERIFY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
	       "malformed framebuffer: {:#x}", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	return true;
}

void Framebuffer::resize(uint32_t width, uint32_t height)
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
	if (m_renderToScreen) {
		return;
	}

	if (m_id) {
		glDeleteFramebuffers(1, &m_id);
		m_textures.clear();
	}

	m_id = UINT_MAX;
	glGenFramebuffers(1, &m_id);

	bind();

	auto it = m_attachments.begin();
	m_colorAttachmentCount = 0;

	size_t size = m_attachments.size();
	m_textures.resize(size);
	for (size_t i = 0; i < size; ++i) {
		TypeProperties type = *(it + i);

		if (type.type == Type::RGB8) {
			// Set color attachment 0 out of 32
			m_textures[i] = TextureFramebuffer::create("", m_width, m_height, GL_RGB8, GL_RGB);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			m_colorAttachmentCount++;
			continue;
		}

		if (type.type == Type::RGBA8) { // Color
			// Set color attachment 0 out of 32
			m_textures[i] = TextureFramebuffer::create("", m_width, m_height, GL_RGBA8, GL_RGBA);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			m_colorAttachmentCount++;
			continue;
		}

		if (type.type == Type::RGBA16F) {
			// Set color attachment 0 out of 32
			m_textures[i] = TextureFramebuffer::create("", m_width, m_height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			m_colorAttachmentCount++;
			continue;
		}

		if (type.type == Type::RGBA32F) {
			// Set color attachment 0 out of 32
			m_textures[i] = TextureFramebuffer::create("", m_width, m_height, GL_RGBA32F, GL_RGBA, GL_FLOAT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			m_colorAttachmentCount++;
			continue;
		}

		// This combined texture is required for older GPUs
		if (type.type == Type::Depth24Stencil8) { // Depth
			m_textures[i] = (TextureFramebuffer::create(
				"", m_width, m_height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8));
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			continue;
		}

		if (type.type == Type::Depth32F) {
			//  FIXME: This isnt a 32-bit float texture
			m_textures[i] = (TextureFramebuffer::create(
				"", m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT));
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[i]->id(), 0);
			continue;
		}
	}

	VERIFY(m_colorAttachmentCount <= 32, "maximum color attachments was exceeded: {}/32", m_colorAttachmentCount);
	check();

	unbind();
}

} // namespace Inferno
