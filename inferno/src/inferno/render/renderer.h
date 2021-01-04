#ifndef RENDERER_H
#define RENDERER_H

#include <memory>  // std::shared_ptr

#include "glm/ext/vector_float4.hpp" // glm::vec4

namespace Inferno {

	class Camera;
	class Shader;
	class Texture;
	class Transform;
	class VertexArray;

	struct RenderBundle {
		const std::shared_ptr<Shader>& shader;
		const std::shared_ptr<Texture>& texture;
		const std::shared_ptr<Transform>& transform;
		const std::shared_ptr<VertexArray>& vertexArray;
	};

	class RenderCommand {
	public:
		static void clear();
		static void clearColor(const glm::vec4& color);

		static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};

// -----------------------------------------

	class Renderer2D {
	public:
		static void beginScene(const std::shared_ptr<Camera>& camera);
		static void endScene();

		static void submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void submit(const RenderBundle& bundle);

	private:
		static std::shared_ptr<Camera> m_camera;
	};

}

#endif // RENDERER_H
