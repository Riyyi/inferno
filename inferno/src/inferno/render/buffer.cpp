#include <glad/glad.h>

#include "inferno/render/buffer.h"

namespace Inferno {

	VertexBuffer::VertexBuffer(float* vertices, size_t size)
	{
		glCreateBuffers(1, &m_id);
		this->bind();

		// Upload data to the GPU
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

// -----------------------------------------

	IndexBuffer::IndexBuffer(uint32_t* indices, size_t size) :
		m_count(size / sizeof(uint32_t))
	{
		glCreateBuffers(1, &m_id);
		this->bind();

		// Upload data to the GPU
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
