#include <algorithm> // std::min
#include <utility>   // std::move

#include "glad/glad.h"

#include "inferno/render/buffer.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"

namespace Inferno {

	void RenderCommand::initialize()
	{
		setDepthTest(true);

		// Enable transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		info() << "RenderCommand initialized";
	}

	void RenderCommand::destroy()
	{
	}

	void RenderCommand::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::clearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::drawIndexed(const VertexArray& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray.getIndexBuffer()->getCount();
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

// -----------------------------------------

	uint32_t Renderer::m_supportedTextureUnitPerBatch = 0;

	void Renderer::initialize()
	{
		// Get amount of texture units supported by the GPU
		uint32_t constTextureUnitCount = textureUnitPerBatch;
		uint32_t gpuTextureUnitCount = RenderCommand::textureUnitAmount();
		m_supportedTextureUnitPerBatch = std::min(constTextureUnitCount, gpuTextureUnitCount);

		// Texture unit 0 is reserved for no texture
		m_textureUnits[0] = nullptr;

		// Create texture unit samplers
		int32_t samplers[textureUnitPerBatch];
		for (uint32_t i = 0; i < textureUnitPerBatch; i++) {
			samplers[i] = i;
		}

		// Create shader
		loadShader();
		m_shader->bind();
		m_shader->setInt("u_textures", samplers, textureUnitPerBatch);
		m_shader->unbind();

		// Create vertex array
		m_vertexArray = std::make_shared<VertexArray>();
	}

	void Renderer::destroy()
	{
	}

	uint32_t Renderer::addTextureUnit(std::shared_ptr<Texture> texture)
	{
		if (texture == nullptr) {
			return 0;
		}

		// Create a new batch if the texture unit limit has been reached
		if (m_textureUnitIndex >= m_supportedTextureUnitPerBatch) {
			nextBatch();
		}

		// If texure was already added
		for (uint32_t i = 1; i < m_textureUnitIndex; i++) {
			if (m_textureUnits[i] == texture) {
				return i;
			}
		}

		// Add texture
		uint32_t textureUnitIndex = m_textureUnitIndex;
		m_textureUnits[textureUnitIndex] = texture;
		m_textureUnitIndex++;

		return textureUnitIndex;
	}

	void Renderer::bind()
	{
		m_shader->bind();

		for (uint32_t i = 1; i < m_textureUnitIndex; i++) {
			m_textureUnits[i]->bind(i);
		}

		m_vertexArray->bind();
	}

	void Renderer::unbind()
	{
		m_vertexArray->unbind();

		for (uint32_t i = 1; i < m_textureUnitIndex; i++) {
			m_textureUnits[i]->unbind();
		}

		m_shader->unbind();
	}

// -----------------------------------------

	Renderer2D::Renderer2D(s)
	{
		Renderer::initialize();

		// CPU
		// ---------------------------------

		// Create array for storing quads vertices
		m_vertexBufferBase = std::make_unique<QuadVertex[]>(vertexCount);
		m_vertexBufferPtr = m_vertexBufferBase.get();

		// Set default quad vertex positions
		m_vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_vertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		m_vertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		m_vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Generate indices

		uint32_t* indices = new uint32_t[indexCount];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < indexCount; i += indexPerQuad) {
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

		// Create vertex buffer
		auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(QuadVertex) * vertexCount);
		vertexBuffer->setLayout({
			{ BufferElementType::Vec3,  "a_position" },
			{ BufferElementType::Vec4,  "a_color" },
			{ BufferElementType::Vec2,  "a_textureCoordinates" },
			{ BufferElementType::Float, "a_textureIndex" },
		});
		m_vertexArray->addVertexBuffer(vertexBuffer);

		// Create index buffer
		auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(uint32_t) * indexCount);
		m_vertexArray->setIndexBuffer(indexBuffer);
		delete[] indices;

		info() << "Renderer2D initialized";
	}

	Renderer2D::~Renderer2D()
	{
		Renderer::destroy();
	}

	void Renderer2D::beginScene(glm::mat4 cameraProjectionView)
	{
		m_shader->bind();
		m_shader->setFloat("u_projectionView", cameraProjectionView);
		m_shader->unbind();
	}

	void Renderer2D::endScene()
	{
		nextBatch();
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
		if (m_quadIndex >= quadCount) {
			nextBatch();
		}

		constexpr glm::vec2 textureCoordinates[] = {
			{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
		};

		uint32_t textureUnitIndex = addTextureUnit(texture);

		// Add the quads 4 vertices
		for (uint32_t i = 0; i < vertexPerQuad; i++) {
			m_vertexBufferPtr->position =  transform.transform * m_vertexPositions[i];
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

	void Renderer2D::flush()
	{
		if (m_quadIndex == 0) {
			return;
		}

		// Upload vertex data to GPU
		m_vertexArray->getVertexBuffers().at(0)->uploadData(
			m_vertexBufferBase.get(),
			m_quadIndex * vertexPerQuad * sizeof(QuadVertex));

		bind();

		// Render
		RenderCommand::drawIndexed(*m_vertexArray, m_quadIndex * indexPerQuad);

		unbind();
	}

	void Renderer2D::startBatch()
	{
		m_quadIndex = 0;
		m_vertexBufferPtr = m_vertexBufferBase.get();

		m_textureUnitIndex = 1;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

// -----------------------------------------

	RendererCharacter::RendererCharacter(s)
	{
		Renderer::initialize();

		// CPU
		// ---------------------------------

		// Create array for storing quads vertices
		m_vertexBufferBase = std::make_unique<CharacterVertex[]>(vertexCount);
		m_vertexBufferPtr = m_vertexBufferBase.get();

		// Generate indices

		uint32_t* indices = new uint32_t[indexCount];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < indexCount; i += indexPerQuad) {
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

		// Create vertex buffer
		auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(CharacterVertex) * vertexCount);
		vertexBuffer->setLayout({
			{ BufferElementType::Vec3,  "a_position" },
			{ BufferElementType::Vec4,  "a_color" },
			{ BufferElementType::Vec2,  "a_textureCoordinates" },
			{ BufferElementType::Float, "a_textureIndex" },
			{ BufferElementType::Float, "a_width" },
			{ BufferElementType::Float, "a_edge" },
			{ BufferElementType::Float, "a_borderWidth" },
			{ BufferElementType::Float, "a_borderEdge" },
			{ BufferElementType::Vec4,  "a_borderColor" },
			{ BufferElementType::Float, "a_offset" },
		});
		m_vertexArray->addVertexBuffer(vertexBuffer);

		// Create index buffer
		auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(uint32_t) * indexCount);
		m_vertexArray->setIndexBuffer(indexBuffer);
		delete[] indices;

		info() << "RendererCharacter initialized";
	}

	RendererCharacter::~RendererCharacter()
	{
		Renderer::destroy();
	}

	void RendererCharacter::beginScene()
	{
	}

	void RendererCharacter::endScene()
	{
		nextBatch();
	}

	void RendererCharacter::drawCharacter(std::array<CharacterVertex, vertexPerQuad>& characterQuad, std::shared_ptr<Texture> texture)
	{
		// Create a new batch if the quad limit has been reached
		if (m_quadIndex >= quadCount) {
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

	void RendererCharacter::flush()
	{
		if (m_quadIndex == 0) {
			return;
		}

		// Upload vertex data to GPU
		m_vertexArray->getVertexBuffers().at(0)->uploadData(
			m_vertexBufferBase.get(),
			m_quadIndex * vertexPerQuad * sizeof(CharacterVertex));

		bind();

		// Render
		bool depthTest = RenderCommand::depthTest();
		RenderCommand::setDepthTest(false);
		RenderCommand::drawIndexed(*m_vertexArray, m_quadIndex * indexPerQuad);
		RenderCommand::setDepthTest(depthTest);

		unbind();
	}

	void RendererCharacter::startBatch()
	{
		m_quadIndex = 0;
		m_vertexBufferPtr = m_vertexBufferBase.get();

		m_textureUnitIndex = 1;
	}

	void RendererCharacter::nextBatch()
	{
		flush();
		startBatch();
	}

}
