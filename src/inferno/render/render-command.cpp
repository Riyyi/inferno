/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr

#include "glad/glad.h"
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/render/buffer.h"
#include "inferno/render/render-command.h"

namespace Inferno {

void RenderCommand::initialize()
{
	setDepthTest(true);

	// Enable transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	ruc::info("RenderCommand initialized");
}

void RenderCommand::destroy()
{
}

void RenderCommand::clearBit(uint32_t bits)
{
	// GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	glClear(bits);
}

void RenderCommand::clearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::drawIndexed(std::shared_ptr<VertexArray> vertexArray, uint32_t indexCount)
{
	uint32_t count = indexCount ? indexCount : vertexArray->indexBuffer()->count();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void RenderCommand::setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void RenderCommand::setDepthTest(bool enabled)
{
	// Set z-buffer / depth buffer
	enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void RenderCommand::setColorAttachmentCount(uint32_t count)
{
	static constexpr uint32_t colorAttachments[] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8,
		GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14,
		GL_COLOR_ATTACHMENT15, GL_COLOR_ATTACHMENT16, GL_COLOR_ATTACHMENT17,
		GL_COLOR_ATTACHMENT18, GL_COLOR_ATTACHMENT19, GL_COLOR_ATTACHMENT20,
		GL_COLOR_ATTACHMENT21, GL_COLOR_ATTACHMENT22, GL_COLOR_ATTACHMENT23,
		GL_COLOR_ATTACHMENT24, GL_COLOR_ATTACHMENT25, GL_COLOR_ATTACHMENT26,
		GL_COLOR_ATTACHMENT27, GL_COLOR_ATTACHMENT28, GL_COLOR_ATTACHMENT29,
		GL_COLOR_ATTACHMENT30, GL_COLOR_ATTACHMENT31
	};
	static constexpr uint32_t maxCount = sizeof(colorAttachments) / sizeof(colorAttachments[0]);
	VERIFY(count > 0 && count <= maxCount, "incorrect colorbuffer count: {}/{}", count, maxCount);
	glDrawBuffers(static_cast<int32_t>(count), colorAttachments); // Multiple Render Targets (MRT)
}

bool RenderCommand::depthTest()
{
	unsigned char depthTest = GL_FALSE;
	glGetBooleanv(GL_DEPTH_TEST, &depthTest);
	return depthTest == GL_TRUE;
}

int32_t RenderCommand::textureUnitAmount()
{
	int32_t amount = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &amount);
	return amount;
}

} // namespace Inferno
