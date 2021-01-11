#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint> // std::uint32_t
#include <memory>  // std::shared_ptr, std::unique_ptr

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float2.hpp"   // glm::vec2
#include "glm/ext/vector_float3.hpp"   // glm::vec3
#include "glm/ext/vector_float4.hpp"   // glm::vec4

namespace Inferno {

	class Camera;
	class Shader;
	class Texture;
	class Transform;
	class VertexArray;

	struct QuadVertex {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 textureCoordinates = { 0.0f, 0.0f };
		float textureIndex = 0; // @Todo get int to pass to fragment correctly
	};

// -----------------------------------------

	class RenderCommand {
	public:
		static void clear();
		static void clearColor(const glm::vec4& color);

		static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);

		static int32_t textureUnitAmount();
	};

// -----------------------------------------

	class Renderer {
	public:
		static const uint32_t vertexPerQuad = 4;
		static const uint32_t indexPerQuad = 6;
		static const uint32_t textureUnitPerBatch = 32;

	protected:
		virtual void initialize() = 0;
		virtual void destroy() = 0;

		uint32_t addTextureUnit(std::shared_ptr<Texture> texture);

		void bind();
		void unbind();
		virtual void flush() = 0;
		virtual void startBatch() = 0;
		virtual void nextBatch() = 0;

		uint32_t m_quadIndex = 0;

		// Texture units
		static uint32_t m_supportedTextureUnitPerBatch;
		uint32_t m_textureUnitIndex = 1;
		std::array<std::shared_ptr<Texture>, textureUnitPerBatch> m_textureUnits;

		// GPU objects
		std::shared_ptr<Shader> m_shader = nullptr;
		std::shared_ptr<VertexArray> m_vertexArray = nullptr;
	};

// -----------------------------------------

	class Renderer2D final : public Renderer {
	public:
		static const uint32_t quadCount = 1000;
		static const uint32_t vertexCount = quadCount * vertexPerQuad;
		static const uint32_t indexCount = quadCount * indexPerQuad;

		void initialize() override;
		void destroy() override;

		void beginScene(const std::shared_ptr<Camera>& camera);
		void endScene();

		void drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color);
		void drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color);
		void drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color, std::shared_ptr<Texture> texture);
		void drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color, std::shared_ptr<Texture> texture);

		static inline Renderer2D& the() { return *s_instance; }

	private:
		void flush() override;
		void startBatch() override;
		void nextBatch() override;

		std::shared_ptr<Camera> s_camera;

		// CPU quad vertices
		std::unique_ptr<QuadVertex[]> m_vertexBufferBase = nullptr;
		QuadVertex* m_vertexBufferPtr = nullptr;

		// Default quad vertex positions
		glm::vec4 m_vertexPositions[vertexPerQuad];

		static Renderer2D* s_instance;
	};

}

#endif // RENDERER_H
