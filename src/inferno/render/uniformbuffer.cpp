/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t, uint8_t
#include <string_view>

#include "glad/glad.h"
#include "glm/ext/matrix_float2x2.hpp" // glm::mat2
#include "glm/ext/matrix_float3x3.hpp" // glm::mat3
#include "glm/gtc/type_ptr.hpp"        // glm::value_ptr
#include "ruc/meta/assert.h"

#include "inferno/render/buffer.h"
#include "inferno/render/uniformbuffer.h"

namespace Inferno {

Uniformbuffer::Uniformbuffer(s)
{
	// Get maximum uniformbuffer bindings the GPU supports
	int32_t maxBindingPoints = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindingPoints);
	m_maxBindingPoints = static_cast<uint8_t>(maxBindingPoints);
}

Uniformbuffer::~Uniformbuffer()
{
	for (const auto& [_, block] : m_blocks) {
		glDeleteBuffers(1, &block.id);
	}
	m_blocks.clear();
}

// -----------------------------------------

void Uniformbuffer::setLayout(std::string_view blockName, const UniformbufferBlock& block)
{
	VERIFY(block.size && block.bindingPoint && block.uniformLocations.size(),
	       "invalid uniformbuffer block definition: {}", blockName);

	VERIFY(block.bindingPoint < m_maxBindingPoints,
	       "uniformbuffer exceeded binding points: {}/{}", block.bindingPoint, m_maxBindingPoints);

	m_blocks[blockName] = block;
}

void Uniformbuffer::setLayout(std::string_view blockName, uint8_t bindingPoint, const BufferLayout& layout)
{
	VERIFY(bindingPoint < m_maxBindingPoints,
	       "uniformbuffer exceeded binding points: {}/{}", bindingPoint, m_maxBindingPoints);

	if (!exists(blockName)) {
		m_blocks[blockName] = {};
	}

	UniformbufferBlock& block = m_blocks[blockName];
	block.bindingPoint = bindingPoint;
	block.uniformLocations.clear();

	// Example block layout:
	// - mat3
	// - float
	// - vec2
	// - vec2
	// - float

	// Chunks, 4 slots, 4 bytes per slot
	// [x][x][x][ ] #1
	// [x][x][x][ ] #2
	// [x][x][x][ ] #3
	// [x][ ][x][x] #4
	// [x][x][x][ ] #5

	size_t chunk = 0;
	uint8_t offset = 0;

	for (auto it = layout.begin(); it != layout.end(); ++it) {
		BufferElementType type = it->type();
		const std::string& name = it->name();

		// Calculate offset
		switch (type) {
			// Scalar 1
		case BufferElementType::Bool:
		case BufferElementType::Int:
		case BufferElementType::Uint:
		case BufferElementType::Float: {
			// Offset
			block.uniformLocations[name] = (chunk * 16) + (offset * 4);
			// Jump
			offset += 1;
			break;
		}
			// Scalar 2
		case BufferElementType::Bool2:
		case BufferElementType::Int2:
		case BufferElementType::Uint2:
		case BufferElementType::Vec2: {
			// Add padding
			if (offset == 1) {
				offset++;
			}
			if (offset == 3) {
				offset = 0;
				chunk++;
			}
			// Offset
			block.uniformLocations[name] = (chunk * 16) + (offset * 4);
			// Jump
			offset += 2;
			break;
		}
			// Scalar 3
		case BufferElementType::Bool3:
		case BufferElementType::Int3:
		case BufferElementType::Uint3:
		case BufferElementType::Vec3: {
			// Add padding
			if (offset != 0) {
				offset = 0;
				chunk++;
			}
			// Offset
			block.uniformLocations[name] = (chunk * 16) + (offset * 4);
			// Jump
			offset += 3;
			break;
		}
			// Scalar 4
		case BufferElementType::Bool4:
		case BufferElementType::Int4:
		case BufferElementType::Uint4:
		case BufferElementType::Vec4: {
			// Add padding
			if (offset != 0) {
				offset = 0;
				chunk++;
			}
			// Offset
			block.uniformLocations[name] = (chunk * 16) + (offset * 4);
			// Jump
			offset += 4;
			break;
		}
			// Array types
		case BufferElementType::Mat2:
		case BufferElementType::Mat3:
		case BufferElementType::Mat4: {
			// Add padding
			if (offset != 0) {
				offset = 0;
				chunk++;
			}
			// Offset
			block.uniformLocations[name] = (chunk * 16) + (offset * 4);

			// Additional rows
			if (type == BufferElementType::Mat2) {
				chunk += 1;
			}
			else if (type == BufferElementType::Mat3) {
				chunk += 2;
			}
			else {
				chunk += 3;
			}

			// Jump
			offset += 4;
			break;
		}
			// TODO: Implement these types
		case BufferElementType::Double:
		case BufferElementType::Vec2Double:
		case BufferElementType::Vec3Double:
		case BufferElementType::Vec4Double:
		case BufferElementType::MatDouble2:
		case BufferElementType::MatDouble3:
		case BufferElementType::MatDouble4:
			VERIFY_NOT_REACHED();
		case BufferElementType::None:
			VERIFY_NOT_REACHED();
		};

		// Overflow slots to next chunk
		if (offset > 3) {
			offset = 0;
			chunk++;
		}
	}

	// Pad the end of the buffer
	if (offset != 0) {
		offset = 0;
		chunk++;
	}

	block.size = chunk * 16;
}

void Uniformbuffer::create(std::string_view blockName)
{
	VERIFY(exists(blockName), "uniformbuffer block doesnt exist");

	UniformbufferBlock& block = m_blocks[blockName];

	if (block.id != 0) {
		glDeleteBuffers(1, &block.id);
	}

	// Allocate buffer
	block.id = UINT_MAX;
	glGenBuffers(1, &block.id);
	glBindBuffer(GL_UNIFORM_BUFFER, block.id);
	glBufferData(GL_UNIFORM_BUFFER, block.size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Bind buffer to binding point
	glBindBufferBase(GL_UNIFORM_BUFFER, block.bindingPoint, block.id);
}

void Uniformbuffer::setValue(std::string_view blockName, std::string_view member, bool value)
{
	setValue(blockName, member, static_cast<uint32_t>(value), sizeof(uint32_t));
}

void Uniformbuffer::setValue(std::string_view blockName, std::string_view member, glm::mat2 value)
{
	setValue(blockName, member, static_cast<glm::mat4>(value), sizeof(glm::vec4) * 2);
}

void Uniformbuffer::setValue(std::string_view blockName, std::string_view member, glm::mat3 value)
{
	setValue(blockName, member, static_cast<glm::mat4>(value), sizeof(glm::vec4) * 3);
}

} // namespace Inferno
