/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef> // size_t
#include <cstdint> // uint8_t, uint32_t
#include <string>
#include <string_view>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/ext/matrix_float2x2.hpp" // glm::mat2
#include "glm/ext/matrix_float3x3.hpp" // glm::mat3
#include "ruc/singleton.h"

#include "inferno/render/buffer.h"

#define CHECK_UNIFORM_SET_CALL(blockName, member)                                      \
	VERIFY(exists(blockName), "uniformbuffer block doesnt exist: {}", blockName);      \
	const UniformbufferBlock& block = m_blocks[blockName];                             \
	VERIFY(block.uniformLocations.find(member.data()) != block.uniformLocations.end(), \
	       "uniformbuffer member doesnt exist: {}", member);

namespace Inferno {

struct UniformbufferBlock {
	uint32_t id { 0 };
	uint32_t size { 0 };
	uint8_t bindingPoint { 0 };
	std::unordered_map<std::string, uint32_t> uniformLocations {};
};

class Uniformbuffer final : public ruc::Singleton<Uniformbuffer> { // Uniform Buffer Object, UBO
public:
	Uniformbuffer(s);
	~Uniformbuffer();

	void setLayout(std::string_view blockName, const UniformbufferBlock& block);
	void setLayout(std::string_view blockName, uint8_t bindingPoint, const BufferLayout& layout);
	void create(std::string_view blockName);

	template<typename T> // Capture value by reference, instead of decaying to pointer
	void setValue(std::string_view blockName, std::string_view member, T&& value, size_t size = 0)
	{
		CHECK_UNIFORM_SET_CALL(blockName, member);

		glBindBuffer(GL_UNIFORM_BUFFER, block.id);
		glBufferSubData(GL_UNIFORM_BUFFER, block.uniformLocations.at(member.data()), (size) ? size : sizeof(T), &value);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	// Exceptions:
	void setValue(std::string_view blockName, std::string_view member, bool value);
	void setValue(std::string_view blockName, std::string_view member, glm::mat2 value);
	void setValue(std::string_view blockName, std::string_view member, glm::mat3 value);

	bool exists(std::string_view blockName) const { return m_blocks.find(blockName) != m_blocks.end(); }

private:
	uint8_t m_maxBindingPoints { 0 };
	std::unordered_map<std::string_view, UniformbufferBlock> m_blocks;
};

} // namespace Inferno

#if 0

// -----------------------------------------
// Example usage:

Uniformbuffer::the().setLayout(
	"ExampleBlock", 0,
	{
		{ BufferElementType::Mat3,  "a" },
		{ BufferElementType::Float, "b" },
		{ BufferElementType::Vec2,  "c" },
		{ BufferElementType::Vec2,  "d" },
		{ BufferElementType::Float, "e" },
	});
Uniformbuffer::the().create("ExampleBlock");

#endif

// -----------------------------------------
// Memory alignment of uniform blocks using std140
//
// Main points:
// - Memory is organized into chunks.
// - A block is at least the size of 1 chunk.
// - One chunk has 4 slots, 4 bytes per slot.
// - Can't fit? Move to next chunk.
//
// The rules:
// 1. Scalar (bool, int, uint, float) takes up 1 slot, can appear after anything
// 2. Vec2 takes up 2 slots, in first or last half of a chunk
// 3. Vec3 takes up 3 slots, only at the start of a chunk
// 4. Everything else:
//      - Take up maxumum room
//      - As often as needed
//      - Add padding as needed
// 5. Mat3 (or any matrix) are treated like arrays
// 6. Each member of *any* array gets its own chunk

// TODO: How do double types work?

// -----------------------------------------
// References:
// - https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
// - https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout
// - https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec2.html (The std140 Layout Rules)
// - https://www.youtube.com/watch?v=JPvbRko9lBg (WebGL 2: Uniform Buffer Objects)
