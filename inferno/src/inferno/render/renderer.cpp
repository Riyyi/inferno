#include <algorithm> // std::min
#include <utility>   // std::move

#include <glad/glad.h>

#include "inferno/component/transform.h"
#include "inferno/render/buffer.h"
#include "inferno/render/camera.h"
#include "inferno/render/renderer.h"
#include "inferno/render/shader.h"
#include "inferno/render/texture.h"

namespace Inferno {

	void RenderCommand::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::clearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
 		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	int32_t RenderCommand::textureUnitAmount()
	{
		int32_t amount = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &amount);
		return amount;
	}

// -----------------------------------------

	std::shared_ptr<Camera> Renderer2D::s_camera = nullptr;
	QuadBatch* Renderer2D::s_quadBatch;

	void Renderer2D::initialize()
	{
		s_quadBatch = new QuadBatch();

// CPU
// -----------------------------------------

		// Create array for storing quads vertices
		s_quadBatch->vertexBufferBase = std::unique_ptr<QuadVertex[]>(new QuadVertex[QuadBatch::vertexCount]);
		s_quadBatch->vertexBufferPtr = s_quadBatch->vertexBufferBase.get();

		// Set default quad vertex positions
		s_quadBatch->vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_quadBatch->vertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_quadBatch->vertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_quadBatch->vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Create texture unit samplers
		int32_t samplers[s_quadBatch->textureUnitPerBatch];
		for (uint32_t i = 0; i < s_quadBatch->textureUnitPerBatch; i++) {
			samplers[i] = i;
		}

		// Get amount of texture units supported by the GPU
		uint32_t constTextureUnitCount = QuadBatch::textureUnitPerBatch;
		uint32_t gpuTextureUnitCount = RenderCommand::textureUnitAmount();
		s_quadBatch->supportedTextureUnitPerBatch = std::min(constTextureUnitCount, gpuTextureUnitCount);

		// Texture unit 0 is reserved for no texture
		s_quadBatch->textureUnits[0] = nullptr;

// GPU
// -----------------------------------------

		// Create shader
		s_quadBatch->shader = std::make_unique<Shader>("assets/glsl/batch-quad");
		s_quadBatch->shader->bind();
		s_quadBatch->shader->setInt("u_textures", samplers, s_quadBatch->textureUnitPerBatch);
		s_quadBatch->shader->unbind();

		// Create vertex array
		s_quadBatch->vertexArray = std::make_shared<VertexArray>();

		// Create vertex buffer
		auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(QuadVertex) * QuadBatch::vertexCount);
		vertexBuffer->setLayout({
			{ BufferElementType::Vec3,  "a_position" },
			{ BufferElementType::Vec4,  "a_color" },
			{ BufferElementType::Vec2,  "a_textureCoordinates" },
			{ BufferElementType::Float, "a_textureIndex" },
		});
		s_quadBatch->vertexArray->addVertexBuffer(std::move(vertexBuffer));

		// Generate indices

		uint32_t* indices = new uint32_t[QuadBatch::indexCount];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < QuadBatch::indexCount; i += QuadBatch::indexPerQuad) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += QuadBatch::vertexPerQuad;
		}

		// Create index buffer
		auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(uint32_t) * QuadBatch::indexCount);
		s_quadBatch->vertexArray->setIndexBuffer(indexBuffer);
		delete[] indices;

		dbg(Log::Info) << "Renderer2D initialized";
	}

	void Renderer2D::destroy()
	{
		delete s_quadBatch;
	}

	void Renderer2D::beginScene(const std::shared_ptr<Camera>& camera)
	{
		s_camera = camera;
	}

	void Renderer2D::endScene()
	{
		nextBatch();
	}

	void Renderer2D::drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color)
	{
		drawQuad(transform, color, nullptr);
	}

	void Renderer2D::drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color)
	{
		drawQuad(transform, color, nullptr);
	}

	void Renderer2D::drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color, std::shared_ptr<Texture> texture)
	{
		drawQuad(transform, glm::mat4(color, color, color, color), texture);
	}

	void Renderer2D::drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color, std::shared_ptr<Texture> texture)
	{
		// Create a new batch if the quad limit has been reached
		if (s_quadBatch->quadCount >= QuadBatch::quads) {
			nextBatch();
		}

		constexpr glm::vec2 textureCoordinates[] = {
			{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
		};

		uint32_t textureUnitIndex = addTextureUnit(texture);

		// Add the quads 4 vertices
		for (uint32_t i = 0; i < QuadBatch::vertexPerQuad; i++) {
			s_quadBatch->vertexBufferPtr->position =  transform->transform() * s_quadBatch->vertexPositions[i];
			s_quadBatch->vertexBufferPtr->color = color[i];
			s_quadBatch->vertexBufferPtr->textureCoordinates = textureCoordinates[i];
			s_quadBatch->vertexBufferPtr->textureIndex = (float)textureUnitIndex;
			s_quadBatch->vertexBufferPtr++;
		}

		s_quadBatch->quadCount++;
	}

	uint32_t Renderer2D::addTextureUnit(std::shared_ptr<Texture> texture)
	{
		if (texture == nullptr) {
			return 0;
		}

		// Create a new batch if the texture unit limit has been reached
		if (s_quadBatch->textureUnitIndex >= s_quadBatch->supportedTextureUnitPerBatch) {
			nextBatch();
		}

		// If texure was already added
		for (uint32_t i = 1; i < s_quadBatch->textureUnitIndex; i++) {
			if (s_quadBatch->textureUnits[i] == texture) {
				return i;
			}
		}

		// Add texture
		uint32_t textureUnitIndex = s_quadBatch->textureUnitIndex;
		s_quadBatch->textureUnits[textureUnitIndex] = texture;
		s_quadBatch->textureUnitIndex++;

		return textureUnitIndex;
	}

	void Renderer2D::bind()
	{
		s_quadBatch->shader->bind();

		for (uint32_t i = 1; i < s_quadBatch->textureUnitIndex; i++) {
			s_quadBatch->textureUnits[i]->bind(i);
		}

		s_quadBatch->vertexArray->bind();
	}

	void Renderer2D::unbind()
	{
		s_quadBatch->vertexArray->unbind();

		for (uint32_t i = 1; i < s_quadBatch->textureUnitIndex; i++) {
			s_quadBatch->textureUnits[i]->unbind();
		}

		s_quadBatch->shader->unbind();
	}

	void Renderer2D::flush()
	{
		if (s_quadBatch->quadCount == 0) {
			return;
		}

		// Upload vertex data to GPU
		s_quadBatch->vertexArray->getVertexBuffers().at(0)->uploadData(
			s_quadBatch->vertexBufferBase.get(),
			s_quadBatch->quadCount * QuadBatch::vertexPerQuad * sizeof(QuadVertex));

		bind();

		// Render
		s_quadBatch->shader->setFloat("u_projectionView", s_camera->projection() * s_camera->transform()->transform());
		RenderCommand::drawIndexed(s_quadBatch->vertexArray, s_quadBatch->quadCount * QuadBatch::indexPerQuad);

		unbind();
	}

	void Renderer2D::startBatch()
	{
		s_quadBatch->quadCount = 0;
		s_quadBatch->vertexBufferPtr = s_quadBatch->vertexBufferBase.get();

		s_quadBatch->textureUnitIndex = 1;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

}
