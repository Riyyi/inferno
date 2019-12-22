#include <glad/glad.h>

#include "inferno/render/buffer.h"
#include "inferno/render/renderer.h"

namespace Inferno {

	void Command::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Command::clearColor(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Command::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

// -----------------------------------------

	void Renderer::beginScene()
	{

	}

	void Renderer::endScene()
	{

	}

	void Renderer::submit(const std::shared_ptr<VertexArray> &vertexArray)
	{
		vertexArray->bind();
		Command::drawIndexed(vertexArray);
	}

}
