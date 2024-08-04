/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <memory> // std::shared_ptr

#include "glad/glad.h"
#include "ruc/format/log.h"

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

void RenderCommand::clearColorDepthBit()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::clearColorBit()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderCommand::clearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::drawIndexed(std::shared_ptr<VertexArray> vertexArray, uint32_t indexCount)
{
	uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
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
