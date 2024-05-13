/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::shadred_ptr

#include "glm/ext/vector_float4.hpp" // glm::vec4

namespace Inferno {

class VertexArray;

class RenderCommand {
public:
	static void initialize();
	static void destroy();

	static void clear();
	static void clearColor(const glm::vec4& color);
	static void drawIndexed(std::shared_ptr<VertexArray> vertexArray, uint32_t indexCount = 0);

	static void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
	static void setDepthTest(bool enabled);

	static bool depthTest();
	static int32_t textureUnitAmount();
};

} // namespace Inferno
