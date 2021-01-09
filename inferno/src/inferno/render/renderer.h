#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint> // std::uint32_t
#include <memory>  // std::shared_ptr

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float2.hpp"   // glm::vec2
#include "glm/ext/vector_float3.hpp"   // glm::vec3
#include "glm/ext/vector_float4.hpp"   // glm::vec4

namespace Inferno {

	class Camera;
	class Shader;
	class Texture;
	class Transform;
	class VertexBuffer;
	class VertexArray;

	struct QuadVertex {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 textureCoordinates = { 0.0f, 0.0f };
		float textureIndex = 0; // @Todo get int to pass to fragment correctly
	};

	struct QuadBatch {
		static const uint32_t vertexPerQuad = 4;
		static const uint32_t indexPerQuad = 6;
		static const uint32_t quads = 1000;
		static const uint32_t vertexCount = quads * vertexPerQuad;
		static const uint32_t indexCount = quads * indexPerQuad;
		static const uint32_t textureUnitPerBatch = 32;

		uint32_t quadCount = 0;
		std::shared_ptr<QuadVertex[]> vertexBufferBase = nullptr;
		QuadVertex* vertexBufferPtr = nullptr;

		glm::vec4 vertexPositions[vertexPerQuad];

		uint32_t supportedTextureUnitPerBatch = 0;
		uint32_t textureUnitIndex = 1;
		std::array<std::shared_ptr<Texture>, textureUnitPerBatch> textureUnits;

		std::shared_ptr<Shader> shader = nullptr;
		std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
		std::shared_ptr<VertexArray> vertexArray = nullptr;
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

	class Renderer2D {
	public:
		static void initialize();
		static void destroy();

		static void beginScene(const std::shared_ptr<Camera>& camera);
		static void endScene();

		static void drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color);
		static void drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color);
		static void drawQuad(std::shared_ptr<Transform> transform, glm::vec4 color, std::shared_ptr<Texture> texture);
		static void drawQuad(std::shared_ptr<Transform> transform, glm::mat4 color, std::shared_ptr<Texture> texture);

	private:
		static uint32_t addTextureUnit(std::shared_ptr<Texture> texture);

		static void bind();
		static void unbind();
		static void flush();
		static void startBatch();
		static void nextBatch();

		static std::shared_ptr<Camera> s_camera;
		static QuadBatch* s_quadBatch;
	};

}

#endif // RENDERER_H
