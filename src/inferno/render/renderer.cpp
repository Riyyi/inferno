/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // std::min
#include <utility>   // std::move

#include "glad/glad.h"
#include "ruc/format/log.h"

#include "inferno/render/buffer.h"
#include "inferno/render/render-command.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"

namespace Inferno {

template<typename T>
void Renderer<T>::beginScene(glm::mat4, glm::mat4)
{
}

template<typename T>
void Renderer<T>::endScene()
{
	nextBatch();
}

// -----------------------------------------

template<typename T>
uint32_t Renderer<T>::m_maxSupportedTextureSlots = 0;

template<typename T>
void Renderer<T>::initialize()
{
	// Get amount of texture units supported by the GPU
	uint32_t gpuTextureUnitCount = RenderCommand::textureUnitAmount();
	m_maxSupportedTextureSlots = std::min(maxTextureSlots, gpuTextureUnitCount);

	// Texture unit 0 is reserved for no texture
	m_textureSlots[0] = nullptr;

	// Create texture unit samplers
	int32_t samplers[maxTextureSlots];
	for (uint32_t i = 0; i < maxTextureSlots; i++) {
		samplers[i] = i;
	}

	// Create shader
	loadShader();
	m_shader->bind();
	m_shader->setInt("u_textures", samplers, maxTextureSlots);
	m_shader->unbind();

	// Create vertex array
	m_vertexArray = std::make_shared<VertexArray>();

	// CPU
	// ---------------------------------

	// Generate indices

	uint32_t* indices = new uint32_t[maxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < maxIndices; i += indexPerQuad) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += vertexPerQuad;
	}

	// GPU
	// ---------------------------------

	// Create index buffer
	auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(uint32_t) * maxIndices);
	m_vertexArray->setIndexBuffer(indexBuffer);
	delete[] indices;
}

template<typename T>
void Renderer<T>::destroy()
{
	delete[] m_vertexBufferBase;
}

template<typename T>
uint32_t Renderer<T>::addTextureUnit(std::shared_ptr<Texture> texture)
{
	if (texture == nullptr) {
		return 0;
	}

	// Create a new batch if the texture unit limit has been reached
	if (m_textureSlotIndex >= m_maxSupportedTextureSlots) {
		nextBatch();
	}

	// If texure was already added
	for (uint32_t i = 1; i < m_textureSlotIndex; i++) {
		if (m_textureSlots[i] == texture) {
			return i;
		}
	}

	// Add texture
	uint32_t textureSlotIndex = m_textureSlotIndex;
	m_textureSlots[textureSlotIndex] = texture;
	m_textureSlotIndex++;

	return textureSlotIndex;
}

template<typename T>
void Renderer<T>::bind()
{
	m_shader->bind();

	for (uint32_t i = 1; i < m_textureSlotIndex; i++) {
		m_textureSlots[i]->bind(i);
	}

	m_vertexArray->bind();
}

template<typename T>
void Renderer<T>::unbind()
{
	m_vertexArray->unbind();

	for (uint32_t i = 1; i < m_textureSlotIndex; i++) {
		m_textureSlots[i]->unbind();
	}

	m_shader->unbind();
}

template<typename T>
void Renderer<T>::flush()
{
	if (m_quadIndex == 0) {
		return;
	}

	// Upload vertex data to GPU
	m_vertexArray->at(0)->uploadData(
		m_vertexBufferBase,
		m_quadIndex * vertexPerQuad * sizeof(T));

	bind();

	// Render
	bool depthTest = RenderCommand::depthTest();
	RenderCommand::setDepthTest(m_enableDepthBuffer);
	RenderCommand::drawIndexed(*m_vertexArray, m_quadIndex * indexPerQuad);
	RenderCommand::setDepthTest(depthTest);

	unbind();
}

template<typename T>
void Renderer<T>::startBatch()
{
	m_quadIndex = 0;
	m_vertexBufferPtr = m_vertexBufferBase;

	m_textureSlotIndex = 1;
}

template<typename T>
void Renderer<T>::nextBatch()
{
	flush();
	startBatch();
}

// -----------------------------------------

Renderer2D::Renderer2D(s)
{
	Renderer::initialize();

	// CPU
	// ---------------------------------

	// Create array for storing quads vertices
	m_vertexBufferBase = new QuadVertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// Set default quad vertex positions
	m_vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	m_vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	m_vertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
	m_vertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

	// GPU
	// ---------------------------------

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(QuadVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Vec2, "a_textureCoordinates" },
		{ BufferElementType::Float, "a_textureIndex" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("Renderer2D initialized");
}

void Renderer2D::beginScene(glm::mat4 cameraProjection, glm::mat4 cameraView)
{
	m_shader->bind();
	m_shader->setFloat("u_projectionView", cameraProjection * cameraView);
	m_shader->unbind();
}

void Renderer2D::drawQuad(const TransformComponent& transform, glm::vec4 color)
{
	drawQuad(transform, color, nullptr);
}

void Renderer2D::drawQuad(const TransformComponent& transform, glm::mat4 color)
{
	drawQuad(transform, color, nullptr);
}

void Renderer2D::drawQuad(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture)
{
	drawQuad(transform, glm::mat4(color, color, color, color), texture);
}

void Renderer2D::drawQuad(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture)
{
	// Create a new batch if the quad limit has been reached
	if (m_quadIndex >= maxQuads) {
		nextBatch();
	}

	constexpr glm::vec2 textureCoordinates[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the quads 4 vertices
	for (uint32_t i = 0; i < vertexPerQuad; i++) {
		m_vertexBufferPtr->position = transform.transform * m_vertexPositions[i];
		m_vertexBufferPtr->color = color[i];
		m_vertexBufferPtr->textureCoordinates = textureCoordinates[i];
		m_vertexBufferPtr->textureIndex = (float)textureUnitIndex;
		m_vertexBufferPtr++;
	}

	m_quadIndex++;
}

void Renderer2D::loadShader()
{
	m_shader = ShaderManager::the().load("assets/glsl/batch-quad");
}

// -----------------------------------------

RendererCubemap::RendererCubemap(s)
{
	Renderer::initialize();

	// CPU
	// ---------------------------------

	// Create array for storing quads vertices
	m_vertexBufferBase = new CubemapVertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// Set default cubemap vertex positions

	// Back face - v
	m_vertexPositions[0] = { 0.5f, -0.5f, 0.5f, 1.0f };
	m_vertexPositions[1] = { -0.5f, -0.5f, 0.5f, 1.0f };
	m_vertexPositions[2] = { -0.5f, 0.5f, 0.5f, 1.0f };
	m_vertexPositions[3] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// Left face - v
	m_vertexPositions[7] = { -0.5f, 0.5f, 0.5f, 1.0f };
	m_vertexPositions[6] = { -0.5f, 0.5f, -0.5f, 1.0f };
	m_vertexPositions[5] = { -0.5f, -0.5f, -0.5f, 1.0f };
	m_vertexPositions[4] = { -0.5f, -0.5f, 0.5f, 1.0f };

	// Right face - v
	m_vertexPositions[8] = { 0.5f, -0.5f, -0.5f, 1.0f };
	m_vertexPositions[9] = { 0.5f, -0.5f, 0.5f, 1.0f };
	m_vertexPositions[10] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_vertexPositions[11] = { 0.5f, 0.5f, -0.5f, 1.0f };

	// Front face - v
	m_vertexPositions[12] = { -0.5f, -0.5f, -0.5f, 1.0f };
	m_vertexPositions[13] = { 0.5f, -0.5f, -0.5f, 1.0f };
	m_vertexPositions[14] = { 0.5f, 0.5f, -0.5f, 1.0f };
	m_vertexPositions[15] = { -0.5f, 0.5f, -0.5f, 1.0f };

	// Top face
	m_vertexPositions[16] = { -0.5f, 0.5f, -0.5f, 1.0f };
	m_vertexPositions[17] = { 0.5f, 0.5f, -0.5f, 1.0f };
	m_vertexPositions[18] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_vertexPositions[19] = { -0.5f, 0.5f, 0.5f, 1.0f };

	// Bottom face
	m_vertexPositions[20] = { -0.5f, -0.5f, -0.5f, 1.0f };
	m_vertexPositions[21] = { -0.5f, -0.5f, 0.5f, 1.0f };
	m_vertexPositions[22] = { 0.5f, -0.5f, 0.5f, 1.0f };
	m_vertexPositions[23] = { 0.5f, -0.5f, -0.5f, 1.0f };

	// GPU
	// ---------------------------------

	m_enableDepthBuffer = false;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(CubemapVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Float, "a_textureIndex" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("RendererCubemap initialized");
}

void RendererCubemap::beginScene(glm::mat4 cameraProjection, glm::mat4 cameraView)
{
	// We want the skybox fixed in position, so only retain the rotation and scale.
	// Set the translation of the camera's view matrix to 0, meaning:
	// x x x 0
	// x x x 0
	// x x x 0
	// 0 0 0 1
	cameraView = glm::mat4(glm::mat3(cameraView));

	m_shader->bind();
	m_shader->setFloat("u_projectionView", cameraProjection * cameraView);
	m_shader->unbind();
}

void RendererCubemap::drawCubemap(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture)
{
	drawCubemap(transform, glm::mat4(color, color, color, color), texture);
}

void RendererCubemap::drawCubemap(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture)
{
	// Create a new batch if the quad limit has been reached
	if (m_quadIndex >= maxQuads) {
		nextBatch();
	}

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the quads 4 vertices, 6 times, once per cube side
	for (uint32_t i = 0; i < vertexPerQuad * quadPerCube; i++) {
		m_vertexBufferPtr->position = transform.transform * m_vertexPositions[i];
		m_vertexBufferPtr->color = color[i % 4];
		m_vertexBufferPtr->textureIndex = (float)textureUnitIndex;
		m_vertexBufferPtr++;
	}

	m_quadIndex += quadPerCube;
}

void RendererCubemap::loadShader()
{
	m_shader = ShaderManager::the().load("assets/glsl/batch-cubemap");
}

// -----------------------------------------

RendererCharacter::RendererCharacter(s)
{
	Renderer::initialize();

	// CPU
	// ---------------------------------

	// Create array for storing quads vertices
	m_vertexBufferBase = new CharacterVertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// GPU
	// ---------------------------------

	m_enableDepthBuffer = false;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(CharacterVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Vec2, "a_textureCoordinates" },
		{ BufferElementType::Float, "a_textureIndex" },
		{ BufferElementType::Float, "a_width" },
		{ BufferElementType::Float, "a_edge" },
		{ BufferElementType::Float, "a_borderWidth" },
		{ BufferElementType::Float, "a_borderEdge" },
		{ BufferElementType::Vec4, "a_borderColor" },
		{ BufferElementType::Float, "a_offset" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("RendererCharacter initialized");
}

void RendererCharacter::drawCharacter(std::array<CharacterVertex, vertexPerQuad>& characterQuad, std::shared_ptr<Texture> texture)
{
	// Create a new batch if the quad limit has been reached
	if (m_quadIndex >= maxQuads) {
		nextBatch();
	}

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the quads 4 vertices
	for (uint32_t i = 0; i < vertexPerQuad; i++) {
		m_vertexBufferPtr->quad.position = characterQuad[i].quad.position;
		m_vertexBufferPtr->quad.color = characterQuad[i].quad.color;
		m_vertexBufferPtr->quad.textureCoordinates = characterQuad[i].quad.textureCoordinates;
		m_vertexBufferPtr->quad.textureIndex = (float)textureUnitIndex;

		m_vertexBufferPtr->width = characterQuad[i].width;
		m_vertexBufferPtr->edge = characterQuad[i].edge;
		m_vertexBufferPtr->borderWidth = characterQuad[i].borderWidth;
		m_vertexBufferPtr->borderEdge = characterQuad[i].borderEdge;
		m_vertexBufferPtr->borderColor = characterQuad[i].borderColor;
		m_vertexBufferPtr->offset = characterQuad[i].offset;

		m_vertexBufferPtr++;
	}

	m_quadIndex++;
}

void RendererCharacter::loadShader()
{
	m_shader = ShaderManager::the().load("assets/glsl/batch-font");
}

} // namespace Inferno
