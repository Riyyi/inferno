/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // std::copy, std::min
#include <span>

#include "glad/glad.h"
#include "glm/ext/vector_float4.hpp" // glm::vec4
#include "ruc/format/log.h"

#include "inferno/asset/asset-manager.h"
#include "inferno/asset/shader.h"
#include "inferno/asset/texture.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/render/buffer.h"
#include "inferno/render/render-command.h"
#include "inferno/render/renderer.h"

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

	createElementBuffer();
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
void Renderer<T>::createElementBuffer()
{
	// ---------------------------------
	// CPU

	// Generate indices

	uint32_t* elements = new uint32_t[maxElements];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < maxElements; i += elementPerQuad) {
		elements[i + 0] = offset + 0;
		elements[i + 1] = offset + 1;
		elements[i + 2] = offset + 2;
		elements[i + 3] = offset + 2;
		elements[i + 4] = offset + 3;
		elements[i + 5] = offset + 0;

		offset += vertexPerQuad;
	}

	// ---------------------------------
	// GPU

	// Create index buffer
	auto indexBuffer = std::make_shared<IndexBuffer>(elements, sizeof(uint32_t) * maxElements);
	m_vertexArray->setIndexBuffer(indexBuffer);
	delete[] elements;
}

template<typename T>
void Renderer<T>::flush()
{
	if (m_vertexIndex == 0 || m_elementIndex == 0) {
		return;
	}

	// Upload index data to GPU
	uploadElementBuffer();

	// Upload vertex data to GPU
	m_vertexArray->at(0)->uploadData(m_vertexBufferBase, m_vertexIndex * sizeof(T));

	bind();

	// Render
	bool depthTest = RenderCommand::depthTest();
	RenderCommand::setDepthTest(m_enableDepthBuffer);
	RenderCommand::setColorAttachmentCount(m_colorAttachmentCount);
	RenderCommand::drawIndexed(m_vertexArray, m_elementIndex);
	RenderCommand::setDepthTest(depthTest);

	unbind();
}

template<typename T>
void Renderer<T>::startBatch()
{
	m_vertexIndex = 0;
	m_elementIndex = 0;
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
	Renderer2D::initialize();
}

void Renderer2D::initialize()
{
	Renderer::initialize();

	// ---------------------------------
	// CPU

	// Create array for storing quads vertices
	m_vertexBufferBase = new QuadVertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// Set default quad vertex positions
	m_vertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
	m_vertexPositions[1] = { 1.0f, -1.0f, 0.0f, 1.0f };
	m_vertexPositions[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
	m_vertexPositions[3] = { -1.0f, 1.0f, 0.0f, 1.0f };

	// ---------------------------------
	// GPU

	m_enableDepthBuffer = false;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(QuadVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Vec2, "a_textureCoordinates" },
		{ BufferElementType::Uint, "a_textureIndex" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("Renderer2D initialized");
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
	if (m_vertexIndex + vertexPerQuad > maxVertices || m_elementIndex + elementPerQuad > maxElements) {
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
		m_vertexBufferPtr->textureIndex = textureUnitIndex;
		m_vertexBufferPtr++;
	}

	m_vertexIndex += vertexPerQuad;
	m_elementIndex += elementPerQuad;
}

void Renderer2D::loadShader()
{
	m_shader = AssetManager::the().load<Shader>("assets/glsl/post-process");
}

// -----------------------------------------

RendererCubemap::RendererCubemap(s)
{
	RendererCubemap::initialize();
}

void RendererCubemap::initialize()
{
	Renderer::initialize();

	// ---------------------------------
	// CPU

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

	// ---------------------------------
	// GPU

	m_enableDepthBuffer = false;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(CubemapVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Uint, "a_textureIndex" },
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
	// cameraView = glm::mat4(glm::mat3(cameraView));

	m_shader->bind();
	m_shader->setFloat("u_projectionView2", cameraProjection * cameraView);
	m_shader->unbind();
}

void RendererCubemap::drawCubemap(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture)
{
	drawCubemap(transform, glm::mat4(color, color, color, color), texture);
}

void RendererCubemap::drawCubemap(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture)
{
	// Create a new batch if the quad limit has been reached
	if (m_vertexIndex + (vertexPerQuad * quadPerCube) > maxVertices
	    || m_elementIndex + (elementPerQuad * quadPerCube) > maxElements) {
		nextBatch();
	}

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the quads 4 vertices, 6 times, once per cube side
	for (uint32_t i = 0; i < vertexPerQuad * quadPerCube; i++) {
		m_vertexBufferPtr->position = transform.transform * m_vertexPositions[i];
		m_vertexBufferPtr->color = color[i % 4];
		m_vertexBufferPtr->textureIndex = textureUnitIndex;
		m_vertexBufferPtr++;
	}

	m_vertexIndex += vertexPerQuad * quadPerCube;
	m_elementIndex += elementPerQuad * quadPerCube;
}

void RendererCubemap::loadShader()
{
	m_shader = AssetManager::the().load<Shader>("assets/glsl/batch-cubemap");
}

// -----------------------------------------

RendererFont::RendererFont(s)
{
	Renderer::initialize();

	// ---------------------------------
	// CPU

	// Create array for storing quads vertices
	m_vertexBufferBase = new SymbolVertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// ---------------------------------
	// GPU

	m_enableDepthBuffer = false;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(SymbolVertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Vec2, "a_textureCoordinates" },
		{ BufferElementType::Uint, "a_textureIndex" },
		{ BufferElementType::Float, "a_width" },
		{ BufferElementType::Float, "a_edge" },
		{ BufferElementType::Float, "a_borderWidth" },
		{ BufferElementType::Float, "a_borderEdge" },
		{ BufferElementType::Vec4, "a_borderColor" },
		{ BufferElementType::Float, "a_offset" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("RendererFont initialized");
}

void RendererFont::drawSymbol(std::array<SymbolVertex, vertexPerQuad>& symbolQuad, std::shared_ptr<Texture> texture)
{
	// Create a new batch if the quad limit has been reached
	if (m_vertexIndex + vertexPerQuad > maxVertices || m_elementIndex + elementPerQuad > maxElements) {
		nextBatch();
	}

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the quads 4 vertices
	for (uint32_t i = 0; i < vertexPerQuad; i++) {
		m_vertexBufferPtr->quad.position = symbolQuad[i].quad.position;
		m_vertexBufferPtr->quad.color = symbolQuad[i].quad.color;
		m_vertexBufferPtr->quad.textureCoordinates = symbolQuad[i].quad.textureCoordinates;
		m_vertexBufferPtr->quad.textureIndex = textureUnitIndex;

		m_vertexBufferPtr->width = symbolQuad[i].width;
		m_vertexBufferPtr->edge = symbolQuad[i].edge;
		m_vertexBufferPtr->borderWidth = symbolQuad[i].borderWidth;
		m_vertexBufferPtr->borderEdge = symbolQuad[i].borderEdge;
		m_vertexBufferPtr->borderColor = symbolQuad[i].borderColor;
		m_vertexBufferPtr->offset = symbolQuad[i].offset;

		m_vertexBufferPtr++;
	}

	m_vertexIndex += vertexPerQuad;
	m_elementIndex += elementPerQuad;
}

void RendererFont::loadShader()
{
	m_shader = AssetManager::the().load<Shader>("assets/glsl/batch-font");
}

// -----------------------------------------

Renderer3D::Renderer3D(s)
{
	Renderer::initialize();

	// ---------------------------------
	// CPU

	// Create array for storing quads vertices
	m_vertexBufferBase = new Vertex[maxVertices];
	m_vertexBufferPtr = m_vertexBufferBase;

	// ---------------------------------
	// GPU

	m_enableDepthBuffer = true;
	m_colorAttachmentCount = 3;

	// Create vertex buffer
	auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(Vertex) * maxVertices);
	vertexBuffer->setLayout({
		{ BufferElementType::Vec3, "a_position" },
		{ BufferElementType::Vec3, "a_normal" },
		{ BufferElementType::Vec4, "a_color" },
		{ BufferElementType::Vec2, "a_textureCoordinates" },
		{ BufferElementType::Uint, "a_textureIndex" },
	});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	ruc::info("Renderer3D initialized");
}

void Renderer3D::drawModel(std::span<const Vertex> vertices, std::span<const uint32_t> elements, const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture)
{
	// ruc::error("drawModel");

	VERIFY(vertices.size() <= maxVertices, "model vertices too big for buffer, {}/{}", vertices.size(), maxVertices);
	VERIFY(elements.size() <= maxElements, "model elements too big for buffer, {}/{}", elements.size(), maxElements);

	// Create a new batch if the quad limit has been reached
	if (m_vertexIndex + vertices.size() > maxVertices || m_elementIndex + elements.size() > maxElements) {
		nextBatch();
	}

	uint32_t textureUnitIndex = addTextureUnit(texture);

	// Add the vertices
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform.transform)));
	for (const auto& vertex : vertices) {
		m_vertexBufferPtr->position = transform.transform * glm::vec4(vertex.position, 1.0f);
		m_vertexBufferPtr->normal = normalMatrix * vertex.normal; // take non-uniform scaling into consideration
		m_vertexBufferPtr->color = color;
		m_vertexBufferPtr->textureCoordinates = vertex.textureCoordinates;
		m_vertexBufferPtr->textureIndex = textureUnitIndex;
		m_vertexBufferPtr++;
	}

	// Copy element indices to the element buffer
	for (const auto& element : elements) {
		// Indices are referenced relative to vertices[0], if there are multiple models in a batch,
		// then the indices need to be offset by the total amount of vertices
		*m_elementBufferPtr++ = element + m_vertexIndex;
	}

	m_vertexIndex += vertices.size();
	m_elementIndex += elements.size();
}

void Renderer3D::createElementBuffer()
{
	// ---------------------------------
	// CPU

	// Create array for storing quads vertices
	m_elementBufferBase = std::make_unique<uint32_t[]>(maxElements);
	m_elementBufferPtr = m_elementBufferBase.get();

	// ---------------------------------
	// GPU

	// Create index buffer
	auto indexBuffer = std::make_shared<IndexBuffer>(m_elementBufferBase.get(), sizeof(uint32_t) * maxElements);
	m_vertexArray->setIndexBuffer(indexBuffer);
}

void Renderer3D::uploadElementBuffer()
{
	m_vertexArray->indexBuffer()->uploadData(m_elementBufferBase.get(), m_elementIndex * sizeof(uint32_t));
}

void Renderer3D::loadShader()
{
	m_shader = AssetManager::the().load<Shader>("assets/glsl/batch-3d");
}

void Renderer3D::startBatch()
{
	Renderer<Vertex>::startBatch();
	m_elementBufferPtr = m_elementBufferBase.get();
}

// -----------------------------------------

void RendererPostProcess::drawQuad(const TransformComponent& transform, std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> position, std::shared_ptr<Texture> normal)
{
	nextBatch();

	constexpr glm::vec2 textureCoordinates[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	uint32_t textureUnitIndex = addTextureUnit(albedo);
	addTextureUnit(position);
	addTextureUnit(normal);

	// Add the quads 4 vertices
	for (uint32_t i = 0; i < vertexPerQuad; i++) {
		m_vertexBufferPtr->position = transform.transform * m_vertexPositions[i];
		m_vertexBufferPtr->textureCoordinates = textureCoordinates[i];
		m_vertexBufferPtr->textureIndex = textureUnitIndex;
		m_vertexBufferPtr++;
	}

	m_vertexIndex += vertexPerQuad;
	m_elementIndex += elementPerQuad;
}

void RendererPostProcess::loadShader()
{
	ruc::error("POSTPROCESSING!");
	m_shader = AssetManager::the().load<Shader>("assets/glsl/post-process");
}

// -----------------------------------------

void RendererLightCube::loadShader()
{
	m_shader = AssetManager::the().load<Shader>("assets/glsl/lightsource");
}

} // namespace Inferno
