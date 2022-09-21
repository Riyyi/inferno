#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr, std::unique_ptr, std::make_shared, std::make_unique

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float2.hpp"   // glm::vec2
#include "glm/ext/vector_float3.hpp"   // glm::vec3
#include "glm/ext/vector_float4.hpp"   // glm::vec4
#include "ruc/singleton.h"

#include "inferno/component/transformcomponent.h"

namespace Inferno {

	class Shader;
	class Texture;
	class VertexArray;

	struct QuadVertex {
		glm::vec3 position { 0.0f, 0.0f, 0.0f };
		glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 textureCoordinates { 0.0f, 0.0f };
		float textureIndex = 0; // @Todo get int to pass to fragment correctly
	};

	struct CharacterVertex {
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

	class RenderCommand {
	public:
		static void initialize();
		static void destroy();

		static void clear();
		static void clearColor(const glm::vec4& color);
		static void drawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0);

		static void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
		static void setDepthTest(bool enabled);

		static bool depthTest();
		static int32_t textureUnitAmount();
	};

	// -------------------------------------

	class Renderer {
	public:
		static const uint32_t vertexPerQuad = 4;
		static const uint32_t indexPerQuad = 6;
		static const uint32_t textureUnitPerBatch = 32;

	protected:
		Renderer() {}

		void initialize();
		void destroy();

		uint32_t addTextureUnit(std::shared_ptr<Texture> texture);

		void bind();
		void unbind();
		virtual void loadShader() = 0;
		virtual void flush() = 0;
		virtual void startBatch() = 0;
		virtual void nextBatch() = 0;

		uint32_t m_quadIndex = 0;

		// Texture units
		static uint32_t m_supportedTextureUnitPerBatch;
		uint32_t m_textureUnitIndex = 1;
		std::array<std::shared_ptr<Texture>, textureUnitPerBatch> m_textureUnits;

		// GPU objects
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;
	};

	// -------------------------------------

	class Renderer2D final
		: public Renderer
		, public ruc::Singleton<Renderer2D> {
	public:
		Renderer2D(s);
		virtual ~Renderer2D();

		using Singleton<Renderer2D>::destroy;

		static const uint32_t quadCount = 1000;
		static const uint32_t vertexCount = quadCount * vertexPerQuad;
		static const uint32_t indexCount = quadCount * indexPerQuad;

		void beginScene(glm::mat4 cameraProjectionView);
		void endScene();

		void drawQuad(const TransformComponent& transform, glm::vec4 color);
		void drawQuad(const TransformComponent& transform, glm::mat4 color);
		void drawQuad(const TransformComponent& transform, glm::vec4 color, std::shared_ptr<Texture> texture);
		void drawQuad(const TransformComponent& transform, glm::mat4 color, std::shared_ptr<Texture> texture);

	private:
		void loadShader() override;
		void flush() override;
		void startBatch() override;
		void nextBatch() override;

		// CPU quad vertices
		std::unique_ptr<QuadVertex[]> m_vertexBufferBase;
		QuadVertex* m_vertexBufferPtr { nullptr };

		// Default quad vertex positions
		glm::vec4 m_vertexPositions[vertexPerQuad];
	};

	// -------------------------------------

	class RendererCharacter final
		: public Renderer
		, public ruc::Singleton<RendererCharacter> {
	public:
		RendererCharacter(s);
		virtual ~RendererCharacter();

		using Singleton<RendererCharacter>::destroy;

		static const uint32_t quadCount = 1000;
		static const uint32_t vertexCount = quadCount * vertexPerQuad;
		static const uint32_t indexCount = quadCount * indexPerQuad;

		void beginScene();
		void endScene();

		void drawCharacter(std::array<CharacterVertex, vertexPerQuad>& characterQuad, std::shared_ptr<Texture> texture);

	private:
		void loadShader() override;
		void flush() override;
		void startBatch() override;
		void nextBatch() override;

		// CPU quad vertices
		std::unique_ptr<CharacterVertex[]> m_vertexBufferBase;
		CharacterVertex* m_vertexBufferPtr { nullptr };
	};

} // namespace Inferno

#endif // RENDERER_H
