/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glad/glad.h"

#include "inferno/core.h"
#include "inferno/render/buffer.h"
#include "ruc/meta/assert.h"

namespace Inferno {

// -----------------------------------------

BufferElement::BufferElement(BufferElementType type, std::string name, bool normalized)
	: m_type(type)
	, m_name(name)
	, m_size(BufferElement::getTypeSize(type))
	, m_normalized(normalized)
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
	case BufferElementType::None:
		return 0;
	case BufferElementType::Bool:
		return sizeof(bool);
	case BufferElementType::Bool2:
		return sizeof(bool) * 2;
	case BufferElementType::Bool3:
		return sizeof(bool) * 3;
	case BufferElementType::Bool4:
		return sizeof(bool) * 4;
	case BufferElementType::Int:
		return sizeof(int32_t);
	case BufferElementType::Int2:
		return sizeof(int32_t) * 2;
	case BufferElementType::Int3:
		return sizeof(int32_t) * 3;
	case BufferElementType::Int4:
		return sizeof(int32_t) * 4;
	case BufferElementType::Uint:
		return sizeof(uint32_t);
	case BufferElementType::Uint2:
		return sizeof(uint32_t) * 2;
	case BufferElementType::Uint3:
		return sizeof(uint32_t) * 3;
	case BufferElementType::Uint4:
		return sizeof(uint32_t) * 4;
	case BufferElementType::Float:
		return sizeof(float);
	case BufferElementType::Vec2:
		return sizeof(float) * 2;
	case BufferElementType::Vec3:
		return sizeof(float) * 3;
	case BufferElementType::Vec4:
		return sizeof(float) * 4;
	case BufferElementType::Double:
		return sizeof(double);
	case BufferElementType::Double2:
		return sizeof(double);
	case BufferElementType::Double3:
		return sizeof(double);
	case BufferElementType::Double4:
		return sizeof(double);
	case BufferElementType::Mat2:
		return sizeof(float) * 2 * 2;
	case BufferElementType::Mat3:
		return sizeof(float) * 3 * 3;
	case BufferElementType::Mat4:
		return sizeof(float) * 4 * 4;
	case BufferElementType::DoubleMat2:
		return sizeof(double) * 2 * 2;
	case BufferElementType::DoubleMat3:
		return sizeof(double) * 3 * 3;
	case BufferElementType::DoubleMat4:
		return sizeof(double) * 4 * 4;
	};

	VERIFY(false, "BufferElement unknown BufferElementType size!");
	return 0;
}

uint32_t BufferElement::getTypeCount(const BufferElementType type)
{
	switch (type) {
	case BufferElementType::None:
		return 0;
	case BufferElementType::Bool:
		return 1;
	case BufferElementType::Bool2:
		return 2;
	case BufferElementType::Bool3:
		return 3;
	case BufferElementType::Bool4:
		return 4;
	case BufferElementType::Int:
		return 1;
	case BufferElementType::Int2:
		return 2;
	case BufferElementType::Int3:
		return 3;
	case BufferElementType::Int4:
		return 4;
	case BufferElementType::Uint:
		return 1;
	case BufferElementType::Uint2:
		return 2;
	case BufferElementType::Uint3:
		return 3;
	case BufferElementType::Uint4:
		return 4;
	case BufferElementType::Float:
		return 1;
	case BufferElementType::Vec2:
		return 2;
	case BufferElementType::Vec3:
		return 3;
	case BufferElementType::Vec4:
		return 4;
	case BufferElementType::Double:
		return 1;
	case BufferElementType::Double2:
		return 2;
	case BufferElementType::Double3:
		return 3;
	case BufferElementType::Double4:
		return 4;
	case BufferElementType::Mat2:
		return 2 * 2;
	case BufferElementType::Mat3:
		return 3 * 3;
	case BufferElementType::Mat4:
		return 4 * 4;
	case BufferElementType::DoubleMat2:
		return 2 * 2;
	case BufferElementType::DoubleMat3:
		return 3 * 3;
	case BufferElementType::DoubleMat4:
		return 4 * 4;
	};

	VERIFY(false, "BufferElement unknown BufferElementType count!");
	return 0;
}

uint32_t BufferElement::getTypeGL(const BufferElementType type)
{
	switch (type) {
	case BufferElementType::None:
		return GL_NONE;
	case BufferElementType::Bool:
		return GL_BOOL;
	case BufferElementType::Bool2:
		return GL_BOOL;
	case BufferElementType::Bool3:
		return GL_BOOL;
	case BufferElementType::Bool4:
		return GL_BOOL;
	case BufferElementType::Int:
		return GL_INT;
	case BufferElementType::Int2:
		return GL_INT;
	case BufferElementType::Int3:
		return GL_INT;
	case BufferElementType::Int4:
		return GL_INT;
	case BufferElementType::Uint:
		return GL_UNSIGNED_INT;
	case BufferElementType::Uint2:
		return GL_UNSIGNED_INT;
	case BufferElementType::Uint3:
		return GL_UNSIGNED_INT;
	case BufferElementType::Uint4:
		return GL_UNSIGNED_INT;
	case BufferElementType::Float:
		return GL_FLOAT;
	case BufferElementType::Vec2:
		return GL_FLOAT;
	case BufferElementType::Vec3:
		return GL_FLOAT;
	case BufferElementType::Vec4:
		return GL_FLOAT;
	case BufferElementType::Double:
		return GL_DOUBLE;
	case BufferElementType::Double2:
		return GL_DOUBLE;
	case BufferElementType::Double3:
		return GL_DOUBLE;
	case BufferElementType::Double4:
		return GL_DOUBLE;
	case BufferElementType::Mat2:
		return GL_FLOAT;
	case BufferElementType::Mat3:
		return GL_FLOAT;
	case BufferElementType::Mat4:
		return GL_FLOAT;
	case BufferElementType::DoubleMat2:
		return GL_DOUBLE;
	case BufferElementType::DoubleMat3:
		return GL_DOUBLE;
	case BufferElementType::DoubleMat4:
		return GL_DOUBLE;
	};

	VERIFY(false, "BufferElement unknown BufferElementType GL!");
	return 0;
}

// -----------------------------------------

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	: m_elements(elements)
{
	calculateOffsetsAndStride();
}

void BufferLayout::calculateOffsetsAndStride()
{
	m_stride = 0;
	for (auto& element : m_elements) {
		element.setOffset(m_stride);
		m_stride += element.getSize();
	}
}

// -----------------------------------------

VertexBuffer::VertexBuffer(size_t size)
{
	glGenBuffers(1, &m_id);
	bind();

	// Reserve data on the GPU
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	unbind();
}

VertexBuffer::VertexBuffer(float* vertices, size_t size)
{
	glGenBuffers(1, &m_id);
	bind();

	// Upload data to the GPU
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	unbind();
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

void VertexBuffer::uploadData(const void* data, uint32_t size)
{
	bind();

	// Upload data to the GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	unbind();
}

// -----------------------------------------

IndexBuffer::IndexBuffer(uint32_t* indices, size_t size)
	: m_count(size / sizeof(uint32_t))
{
	glCreateBuffers(1, &m_id);
	bind();

	// Upload data to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

	unbind();
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

void IndexBuffer::uploadData(const void* data, uint32_t size)
{
	bind();

	// Upload data to the GPU
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);

	unbind();
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

void VertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	const auto& layout = vertexBuffer->getLayout();
	VERIFY(layout.getElements().size(), "VertexBuffer has no layout");

	bind();
	vertexBuffer->bind();

	uint32_t index = 0;
	for (const auto& element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.getTypeCount(),
			element.getTypeGL(),
			element.getNormalized() ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			reinterpret_cast<const void*>(element.getOffset()));

		index++;
	}
	m_vertexBuffers.push_back(std::move(vertexBuffer));

	unbind();
	vertexBuffer->unbind();
}

void VertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	bind();
	indexBuffer->bind();

	m_indexBuffer = std::move(indexBuffer);

	unbind();
	indexBuffer->unbind();
}

} // namespace Inferno
