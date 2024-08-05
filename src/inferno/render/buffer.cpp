/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "glad/glad.h"
#include "ruc/meta/assert.h"

#include "inferno/render/buffer.h"

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
	case BufferElementType::Bool2:
	case BufferElementType::Bool3:
	case BufferElementType::Bool4:
		return sizeof(bool) * getTypeCount(type);
	case BufferElementType::Int:
	case BufferElementType::Int2:
	case BufferElementType::Int3:
	case BufferElementType::Int4:
		return sizeof(int32_t) * getTypeCount(type);
	case BufferElementType::Uint:
	case BufferElementType::Uint2:
	case BufferElementType::Uint3:
	case BufferElementType::Uint4:
		return sizeof(uint32_t) * getTypeCount(type);
	case BufferElementType::Float:
	case BufferElementType::Vec2:
	case BufferElementType::Vec3:
	case BufferElementType::Vec4:
		return sizeof(float) * getTypeCount(type);
	case BufferElementType::VecDouble:
	case BufferElementType::VecDouble2:
	case BufferElementType::VecDouble3:
	case BufferElementType::VecDouble4:
		return sizeof(double) * getTypeCount(type);
	case BufferElementType::Mat2:
	case BufferElementType::Mat3:
	case BufferElementType::Mat4:
		return sizeof(float) * getTypeCount(type);
	case BufferElementType::MatDouble2:
	case BufferElementType::MatDouble3:
	case BufferElementType::MatDouble4:
		return sizeof(double) * getTypeCount(type);
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
	case BufferElementType::Int:
	case BufferElementType::Uint:
	case BufferElementType::Float:
	case BufferElementType::VecDouble:
		return 1;
	case BufferElementType::Bool2:
	case BufferElementType::Int2:
	case BufferElementType::Uint2:
	case BufferElementType::Vec2:
	case BufferElementType::VecDouble2:
		return 2;
	case BufferElementType::Bool3:
	case BufferElementType::Int3:
	case BufferElementType::Uint3:
	case BufferElementType::Vec3:
	case BufferElementType::VecDouble3:
		return 3;
	case BufferElementType::Bool4:
	case BufferElementType::Int4:
	case BufferElementType::Uint4:
	case BufferElementType::Vec4:
	case BufferElementType::VecDouble4:
		return 4;
	case BufferElementType::Mat2:
		return 2 * 2;
	case BufferElementType::Mat3:
		return 3 * 3;
	case BufferElementType::Mat4:
		return 4 * 4;
	case BufferElementType::MatDouble2:
		return 2 * 2;
	case BufferElementType::MatDouble3:
		return 3 * 3;
	case BufferElementType::MatDouble4:
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
	case BufferElementType::Bool2:
	case BufferElementType::Bool3:
	case BufferElementType::Bool4:
		return GL_BOOL;
	case BufferElementType::Int:
	case BufferElementType::Int2:
	case BufferElementType::Int3:
	case BufferElementType::Int4:
		return GL_INT;
	case BufferElementType::Uint:
	case BufferElementType::Uint2:
	case BufferElementType::Uint3:
	case BufferElementType::Uint4:
		return GL_UNSIGNED_INT;
	case BufferElementType::Float:
	case BufferElementType::Vec2:
	case BufferElementType::Vec3:
	case BufferElementType::Vec4:
		return GL_FLOAT;
	case BufferElementType::VecDouble:
	case BufferElementType::VecDouble2:
	case BufferElementType::VecDouble3:
	case BufferElementType::VecDouble4:
		return GL_DOUBLE;
	case BufferElementType::Mat2:
	case BufferElementType::Mat3:
	case BufferElementType::Mat4:
		return GL_FLOAT;
	case BufferElementType::MatDouble2:
	case BufferElementType::MatDouble3:
	case BufferElementType::MatDouble4:
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
	: VertexBuffer(size, nullptr)
{
}

VertexBuffer::VertexBuffer(size_t size, float* vertices)
{
	m_id = UINT_MAX;
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
	m_id = UINT_MAX;
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
	m_id = UINT_MAX;
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
