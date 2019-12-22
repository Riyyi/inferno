#include <glad/glad.h>

#include "inferno/core.h"
#include "inferno/log.h"
#include "inferno/render/buffer.h"

namespace Inferno {

// -----------------------------------------

	BufferElement::BufferElement(BufferElementType type, std::string name, bool normalized) :
			m_type(type),
			m_name(name),
			m_size(BufferElement::getTypeSize(type)),
			m_offset(0),
			m_normalized(normalized)
	{
	}

	uint32_t BufferElement::getTypeSize() const
	{
		return BufferElement::getTypeSize(m_type);
	}

	uint32_t BufferElement::getTypeCount() const
	{
		return BufferElement::getTypeCount(m_type);
	}

	uint32_t BufferElement::getTypeGL() const
	{
		return BufferElement::getTypeGL(m_type);
	}

	uint32_t BufferElement::getTypeSize(const BufferElementType type)
	{
		switch (type) {
			case BufferElementType::None: return 0;
			case BufferElementType::Int:  return sizeof(int32_t);
			case BufferElementType::Int2: return sizeof(int32_t) * 2;
			case BufferElementType::Int3: return sizeof(int32_t) * 3;
			case BufferElementType::Int4: return sizeof(int32_t) * 4;
			case BufferElementType::Mat3: return sizeof(float) * 3 * 3;
			case BufferElementType::Mat4: return sizeof(float) * 4 * 4;
			case BufferElementType::Vec:  return sizeof(float);
			case BufferElementType::Vec2: return sizeof(float) * 2;
			case BufferElementType::Vec3: return sizeof(float) * 3;
			case BufferElementType::Vec4: return sizeof(float) * 4;
		};

		NF_CORE_ASSERT(false, "BufferElement unknown BufferElementType size!");
		return 0;
	}

	uint32_t BufferElement::getTypeCount(const BufferElementType type)
	{
		switch (type) {
			case BufferElementType::None: return 0;
			case BufferElementType::Int:  return 1;
			case BufferElementType::Int2: return 2;
			case BufferElementType::Int3: return 3;
			case BufferElementType::Int4: return 4;
			case BufferElementType::Mat3: return 3 * 3;
			case BufferElementType::Mat4: return 4 * 4;
			case BufferElementType::Vec:  return 1;
			case BufferElementType::Vec2: return 2;
			case BufferElementType::Vec3: return 3;
			case BufferElementType::Vec4: return 4;
		};

		NF_CORE_ASSERT(false, "BufferElement unknown BufferElementType count!");
		return 0;
	}

	uint32_t BufferElement::getTypeGL(const BufferElementType type)
	{
		switch (type) {
			case BufferElementType::None: return GL_NONE;
			case BufferElementType::Int:  return GL_INT;
			case BufferElementType::Int2: return GL_INT;
			case BufferElementType::Int3: return GL_INT;
			case BufferElementType::Int4: return GL_INT;
			case BufferElementType::Mat3: return GL_FLOAT;
			case BufferElementType::Mat4: return GL_FLOAT;
			case BufferElementType::Vec:  return GL_FLOAT;
			case BufferElementType::Vec2: return GL_FLOAT;
			case BufferElementType::Vec3: return GL_FLOAT;
			case BufferElementType::Vec4: return GL_FLOAT;
		};

		NF_CORE_ASSERT(false, "BufferElement unknown BufferElementType GL!");
		return 0;
	}

// -----------------------------------------

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &elements) :
		m_elements(elements), m_stride(0)
	{
		this->calculateOffsetsAndStride();
	}

	void BufferLayout::calculateOffsetsAndStride()
	{
		m_stride = 0;
		for (auto &element : m_elements) {
			element.setOffset(m_stride);
			m_stride += element.getSize();
		}
	}

// -----------------------------------------

	VertexBuffer::VertexBuffer(float* vertices, size_t size)
	{
		glCreateBuffers(1, &m_id);
		this->bind();

		// Upload data to the GPU
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		this->unbind();
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

		this->unbind();
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

// -----------------------------------------

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
	{
		this->bind();
		vertexBuffer->bind();

		uint32_t index = 0;
		for (const auto &element : vertexBuffer->getLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.getTypeCount(),
				element.getTypeGL(),
				element.getNormalized() ? GL_TRUE : GL_FALSE,
				vertexBuffer->getLayout().getStride(),
				(const void*)element.getOffset());

			index++;
		}
		m_vertexBuffers.push_back(vertexBuffer);

		this->unbind();
		vertexBuffer->unbind();
	}

	void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
	{
		this->bind();
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;

		this->unbind();
		indexBuffer->unbind();
	}

}
