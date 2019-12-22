#ifndef RENDERER_H
#define RENDERER_H

#include <memory>  // std::shared_ptr

#include <glm/vec4.hpp>

namespace Inferno {

	class VertexArray;

	class Command {
	public:
		static void clear();
		static void clearColor(const glm::vec4 &color);

		static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray);
	};

// -----------------------------------------

	class Renderer {
	public:
		static void beginScene();
		static void endScene();

		static void submit(const std::shared_ptr<VertexArray> &vertexArray);
	};

}

#endif // RENDERER_H
