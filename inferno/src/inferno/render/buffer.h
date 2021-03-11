#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t
#include <memory>  // std::shared_ptr
#include <string>  // std::string
#include <vector>  // std::vector

namespace Inferno {

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

		inline BufferElementType getType() const{ return m_type; }
		inline std::string getName() const { return m_name; }
		inline uint32_t getSize() const { return m_size; }
		inline uint32_t getOffset() const { return m_offset; }
		inline bool getNormalized() const { return m_normalized; }

		inline void setType(const BufferElementType& type) { m_type = type; }
		inline void setName(const std::string& name) { m_name = name; }
		inline void setSize(const uint32_t& size) { m_size = size; }
		inline void setOffset(const uint32_t& offset) { m_offset = offset; }
		inline void setNormalized(const bool& normalized) { m_normalized = normalized; }

	private:
		BufferElementType m_type;
		std::string m_name;
		uint32_t m_size;
		uint32_t m_offset;
		bool m_normalized;
	};

// -----------------------------------------

	// Layout that describes raw vertex data
	class BufferLayout {
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline uint32_t getStride() const { return m_stride; }

		// Iterators
		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	protected:
		void calculateOffsetsAndStride();

	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};

// -----------------------------------------

	// GPU memory which holds raw vertex data
	class VertexBuffer {
	public:
		VertexBuffer(size_t size);
		VertexBuffer(float* vertices, size_t size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void uploadData(const void* data, uint32_t size);

		inline const BufferLayout& getLayout() const { return m_layout; }

		inline void setLayout(const BufferLayout& layout) { m_layout = layout; }

	private:
		uint32_t m_id;
		BufferLayout m_layout;
	};

// -----------------------------------------

	// Vertices order of rendering
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, size_t size);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_id;
		uint32_t m_count;
	};

// -----------------------------------------

	// Array that holds the vertex attributes configuration
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

		inline const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
		inline std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; }

	private:
		uint32_t m_id;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}

#endif // BUFFER_H
