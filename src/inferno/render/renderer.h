/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr, std::unique_ptr, std::make_shared, std::make_unique

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float2.hpp"   // glm::vec2
#include "glm/ext/vector_float3.hpp"   // glm::vec3
#include "glm/ext/vector_float4.hpp"   // glm::vec4
#include "ruc/singleton.h"

namespace Inferno {

class Shader;
class Texture;
class TransformComponent;
class VertexArray;

struct QuadVertex {
	glm::vec3 position { 0.0f, 0.0f, 0.0f };
	glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 textureCoordinates { 0.0f, 0.0f };
	float textureIndex = 0;
};

struct CubemapVertex {
	glm::vec3 position { 0.0f, 0.0f, 0.0f };
	glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
	float textureIndex = 0;
};

struct SymbolVertex {
	QuadVertex quad;

	// Font
	float width = 0.44f;
	float edge = 0.15f;
	// Outline
	float borderWidth = 0.7f;
	float borderEdge = 0.1f;
	glm::vec4 borderColor { 1.0f, 1.0f, 1.0f, 1.0f };
	// Dropshadow
	float offset = 0.0f;
};

// -------------------------------------

template<typename T>
class Renderer {
public:
	static constexpr const uint32_t vertexPerQuad = 4;
	static constexpr const uint32_t indexPerQuad = 6;
	static constexpr const uint32_t quadPerCube = 6;

	// When to start a new batch
	static constexpr const uint32_t maxQuads = 20000;
	static constexpr const uint32_t maxVertices = maxQuads * vertexPerQuad;
	static constexpr const uint32_t maxIndices = maxQuads * indexPerQuad;
	static constexpr const uint32_t maxTextureSlots = 32;

	virtual void beginScene(glm::mat4 cameraProjection, glm::mat4 cameraView);
	virtual void endScene();

protected:
	Renderer() {}
	virtual ~Renderer() { destroy(); };

	void initialize();
	void destroy();

	uint32_t addTextureUnit(std::shared_ptr<Texture> texture);

	void bind();
	void unbind();

	virtual void loadShader() = 0;
	virtual void flush();
	virtual void startBatch();
	virtual void nextBatch();

protected:
	// CPU quad vertices
	uint32_t m_quadIndex { 0 };
	T* m_vertexBufferBase { nullptr };
	T* m_vertexBufferPtr { nullptr };

	// Texture units
	static uint32_t m_maxSupportedTextureSlots;
	uint32_t m_textureSlotIndex { 1 };
	std::array<std::shared_ptr<Texture>, maxTextureSlots> m_textureSlots;

	// GPU objects
	bool m_enableDepthBuffer { true };
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_vertexArray;
};

// TOOD:
// - Deduplicate flush()
//   v Add bool for disabling depth buffer
//   - Add Size for uploadData (this is prob not needed, we got T already)
// - Decide if its worth to remove template<T> from Renderer, just cast vertexBufferPtr before usage

// -------------------------------------

class Renderer2D final
	: public Renderer<QuadVertex>
	, public ruc::Singleton<Renderer2D> {
public:
	Renderer2D(s);
	virtual ~Renderer2D() {};

	using Singleton<Renderer2D>::destroy;

	virtual void beginScene(glm::mat4 cameraProjection, glm::mat4 cameraView) override;

	void drawQuad(const TransformComponent& transform, glm::vec4 color);
	void drawQuad(const TransformComponent& transform, glm::mat4 color);
	void drawQuad(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture);
	void drawQuad(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture);

private:
	void loadShader() override;

	// Default quad vertex positions
	glm::vec4 m_vertexPositions[vertexPerQuad];
};
// -------------------------------------

class RendererCubemap final
	: public Renderer<CubemapVertex>
	, public ruc::Singleton<RendererCubemap> {
public:
	RendererCubemap(s);
	virtual ~RendererCubemap() {};

	using Singleton<RendererCubemap>::destroy;

	virtual void beginScene(glm::mat4 cameraProjection, glm::mat4 cameraView) override;

	void drawCubemap(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture);
	void drawCubemap(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture);

private:
	void loadShader() override;

	// Default cubemap vertex positions
	glm::vec4 m_vertexPositions[vertexPerQuad * quadPerCube];
};

// -------------------------------------

class RendererFont final
	: public Renderer<SymbolVertex>
	, public ruc::Singleton<RendererFont> {
public:
	RendererFont(s);
	virtual ~RendererFont() {};

	using Singleton<RendererFont>::destroy;

	void drawSymbol(std::array<SymbolVertex, vertexPerQuad>& symbolQuad, std::shared_ptr<Texture> texture);

private:
	void loadShader() override;
};

} // namespace Inferno
