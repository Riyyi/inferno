#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef> // std::size_t
#include <cstdint> // std::uint32_t

namespace Inferno {

	// GPU memory which holds the raw vertex data
	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, size_t size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

	private:
		uint32_t m_id;
	};

// -----------------------------------------

	// Vertices order of rendering
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, size_t size);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

// -----------------------------------------

		inline uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_id;
		uint32_t m_count;
	};

}

#endif // BUFFER_H
