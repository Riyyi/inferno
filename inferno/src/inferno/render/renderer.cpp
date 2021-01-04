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

	void RenderCommand::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

// -----------------------------------------

	std::shared_ptr<Camera> Renderer2D::m_camera = nullptr;

	void Renderer2D::beginScene(const std::shared_ptr<Camera>& camera)
	{
		m_camera = camera;
	}

	void Renderer2D::endScene()
	{

	}

	void Renderer2D::submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
		vertexArray->unbind();
	}

	void Renderer2D::submit(const RenderBundle& bundle)
	{
		bundle.shader->bind();
		if (bundle.texture) bundle.texture->bind();
		bundle.vertexArray->bind();

		bundle.shader->setFloat("u_projectionView", m_camera->projection() * m_camera->transform()->transform());
		bundle.shader->setFloat("u_model", bundle.transform->transform());
		RenderCommand::drawIndexed(bundle.vertexArray);

		bundle.vertexArray->unbind();
		if (bundle.texture) bundle.texture->unbind();
		bundle.shader->unbind();
	}

}
