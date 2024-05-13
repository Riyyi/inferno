/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr
#include <string>  // std::string
#include <vector>  // std::vector

namespace Inferno {

// clang-format off
// https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
enum class BufferElementType {
	None = 0,
	Bool, Bool2, Bool3, Bool4,          // bvec
	Int, Int2, Int3, Int4,              // ivec
	Uint, Uint2, Uint3, Uint4,          // uvec
	Float, Vec2, Vec3, Vec4,            // vec
	Double, Double2, Double3, Double4,  // dvec
	Mat2, Mat3, Mat4,                   // mat
	DoubleMat2, DoubleMat3, DoubleMat4, // dmat
};
// clang-format on

// -----------------------------------------

// Describes one element of the BufferLayout
class BufferElement {
public:
	BufferElement(BufferElementType type, std::string name, bool normalized = false);

	uint32_t getTypeSize() const;
	uint32_t getTypeCount() const;
	uint32_t getTypeGL() const;
	static uint32_t getTypeSize(const BufferElementType type);
	static uint32_t getTypeCount(const BufferElementType type);
	static uint32_t getTypeGL(const BufferElementType type);

	BufferElementType getType() const { return m_type; }
	std::string getName() const { return m_name; }
	uint32_t getSize() const { return m_size; }
	uint32_t getOffset() const { return m_offset; }
	bool getNormalized() const { return m_normalized; }

	void setType(const BufferElementType& type) { m_type = type; }
	void setName(const std::string& name) { m_name = name; }
	void setSize(const uint32_t& size) { m_size = size; }
	void setOffset(const uint32_t& offset) { m_offset = offset; }
	void setNormalized(const bool& normalized) { m_normalized = normalized; }

private:
	BufferElementType m_type;
	std::string m_name;
	uint32_t m_size { 0 };
	uint32_t m_offset { 0 };
	bool m_normalized { false };
};

// -----------------------------------------

// Layout that describes raw vertex data
class BufferLayout {
public:
	BufferLayout() {}
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	const std::vector<BufferElement>& getElements() const { return m_elements; }
	uint32_t getStride() const { return m_stride; }

	// Iterators
	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

protected:
	void calculateOffsetsAndStride();

private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride { 0 };
};

// -----------------------------------------

// GPU memory which holds raw vertex data
class VertexBuffer { // Vertex Buffer Object, VBO
public:
	VertexBuffer(size_t size);
	VertexBuffer(float* vertices, size_t size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	void uploadData(const void* data, uint32_t size);

	const BufferLayout& getLayout() const { return m_layout; }

	inline void setLayout(const BufferLayout& layout) { m_layout = layout; }

private:
	uint32_t m_id { 0 };
	BufferLayout m_layout;
};

// -----------------------------------------

// Vertices order of rendering
class IndexBuffer { // Element Buffer Object, EBO
public:
	IndexBuffer(uint32_t* indices, size_t size);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	void uploadData(const void* data, uint32_t size);

	uint32_t getCount() const { return m_count; }

private:
	uint32_t m_id { 0 };
	uint32_t m_count { 0 };
};

// -----------------------------------------

// Array that holds the vertex attributes configuration
class VertexArray { // Vertex Array Object, VAO
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

	std::shared_ptr<VertexBuffer> at(size_t i) const { return m_vertexBuffers.at(i); }
	std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; }

private:
	uint32_t m_id { 0 };
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
};
} // namespace Inferno
