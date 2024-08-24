/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t, uint8_t, uintt32_t

#include "glad/glad.h"
#include "glm/ext/matrix_float2x2.hpp" // glm::mat2
#include "glm/ext/matrix_float3x3.hpp" // glm::mat3
#include "glm/ext/vector_float4.hpp"   // glm::vec4

#include "inferno/render/buffer.h"
#include "inferno/render/shader-storage-buffer.h"

namespace Inferno {

ShaderStorageBuffer::ShaderStorageBuffer(s)
{
	// Get maximum uniformbuffer bindings the GPU supports
	int32_t maxBindingPoints = 0;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxBindingPoints);
	m_maxBindingPoints = static_cast<uint8_t>(maxBindingPoints);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
}

// -----------------------------------------

// https://stackoverflow.com/questions/56512216#answer-56513136
void ShaderStorageBuffer::setLayout(std::string_view blockName, uint8_t bindingPoint, uint32_t shaderID)
{
	VERIFY(bindingPoint < m_maxBindingPoints,
	       "shader storage buffer exceeded binding points: {}/{}", bindingPoint, m_maxBindingPoints);

	if (!exists(blockName)) {
		m_blocks[blockName.data()] = {};
	}

	BufferBlock& block = m_blocks[blockName.data()];
	block.bindingPoint = bindingPoint;
	block.memberOffsets.clear();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, block.id);

	// Get the shader block index
	uint32_t resourceIndex = glGetProgramResourceIndex(shaderID, GL_SHADER_STORAGE_BLOCK, blockName.data());
	VERIFY(resourceIndex != GL_INVALID_INDEX, "block doesnt exist in shader: {}::{}", blockName, shaderID);

	// Get the amount of member variables
	uint32_t prop = GL_NUM_ACTIVE_VARIABLES;
	int32_t memberCount;
	glGetProgramResourceiv(shaderID, GL_SHADER_STORAGE_BLOCK, resourceIndex, 1, &prop, 1, nullptr, &memberCount);

	// Get the indices of the members
	prop = GL_ACTIVE_VARIABLES;
	std::vector<int32_t> members(memberCount);
	glGetProgramResourceiv(shaderID, GL_SHADER_STORAGE_BLOCK, resourceIndex, 1, &prop, (int32_t)members.size(), nullptr, members.data());

	// Reserve memory for the names of the members
	int32_t memberNameSize;
	glGetProgramInterfaceiv(shaderID, GL_BUFFER_VARIABLE, GL_MAX_NAME_LENGTH, &memberNameSize);
	std::vector<char> memberNameBuffer(memberNameSize);

	int32_t lastOffset = 0;
	int32_t lastType = 0;
	for (int32_t i = 0; i < memberCount; i++) {

		// Get name of buffer variable
		int32_t stringLength;
		glGetProgramResourceName(shaderID, GL_BUFFER_VARIABLE, members[i], memberNameSize, &stringLength, memberNameBuffer.data());
		std::string memberName = std::string(memberNameBuffer.begin(), memberNameBuffer.begin() + stringLength);

		// Get the other data needed for computing

		uint32_t props[7] = {
			GL_OFFSET,                 // 0
			GL_TYPE,                   // 1
			GL_ARRAY_SIZE,             // 2
			GL_ARRAY_STRIDE,           // 3
			GL_MATRIX_STRIDE,          // 4
			GL_TOP_LEVEL_ARRAY_SIZE,   // 5
			GL_TOP_LEVEL_ARRAY_STRIDE, // 6
		};
		int32_t params[7];
		glGetProgramResourceiv(shaderID, GL_BUFFER_VARIABLE, members[i], 7, props, 7, nullptr, params);

		// ruc::error("{}", memberName);
		// ruc::error("\n    Offset: {}, type: {:#x}, arraySize: {}, arrayStride: {},\n    matrixStride: {}, top level size: {}, top level stride: {}\n",
		//            params[0], params[1], params[2], params[3], params[4], params[5], params[6]);

		// Array of structs
		if (params[5] != 1 && params[6 != 0]) {
			size_t bracketOpen = memberName.find_first_of('[');
			size_t bracketClose = memberName.find_first_of(']');
			std::string memberNameBegin = memberName.substr(0, bracketOpen);    // name: myArray[0].member -> myArray
			std::string memberNameMember = memberName.substr(bracketClose + 1); // name: myArray[0].member -> .member
			for (int32_t j = 0; j < params[5]; ++j) {
				lastOffset = params[0] + (j * params[6]); // calc offset
				lastType = params[1];

				// Only add the member variant the first time its encountered
				if (j == 0 && block.memberOffsets.find(memberNameBegin) == block.memberOffsets.end()) {
					block.memberOffsets.emplace(memberNameBegin, lastOffset); // name: myArray
				}

				// Only add the index variant the first time its encountered
				std::string memberNameIndex = memberNameBegin + "[" + std::to_string(j) + "]"; // name: myArray -> myArray[i]
				if (block.memberOffsets.find(memberNameIndex) == block.memberOffsets.end()) {
					block.memberOffsets.emplace(memberNameIndex, lastOffset);
				}

				block.memberOffsets.emplace(memberNameIndex + memberNameMember, // name: myArray -> myArray[i].member
				                            lastOffset);
			}
		}
		// Array of primitives
		else if (params[2] != 1 && params[3] != 0) {
			std::string memberNameBegin = memberName.substr(0, memberName.find_first_of('[')); // name: myArray[0] -> myArray
			for (int32_t j = 0; j < params[2]; ++j) {
				lastOffset = params[0] + (j * params[3]); // calc offset
				lastType = params[1];

				// Only add the member variant the first time its encountered
				if (j == 0) {
					block.memberOffsets.emplace(memberNameBegin, lastOffset); // name: myArray
				}

				block.memberOffsets.emplace(memberNameBegin + "[" + std::to_string(j) + "]", // name: myArray -> myArray[i]
				                            lastOffset);
			}
		}
		// Matrix case
		else if (params[4] != 0) {
			lastType = params[1];
			lastOffset = params[0];
			block.memberOffsets.emplace(memberName, params[0]);
		}
		else {
			lastType = params[1];
			lastOffset = params[0];
			block.memberOffsets.emplace(memberName, params[0]);
		}
	}

	block.size = lastOffset + BufferElement::getGLTypeSize(lastType);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Results
	// for (auto [k, v] : block.memberOffsets) {
	// 	ruc::error("{}:{}", k, v);
	// }
}

void ShaderStorageBuffer::create(std::string_view blockName)
{
	VERIFY(exists(blockName), "shader storage buffer block doesnt exist");

	BufferBlock& block = m_blocks[blockName.data()];

	if (block.id != 0) {
		glDeleteBuffers(1, &block.id);
	}

	// Allocate buffer
	block.id = UINT_MAX;
	glGenBuffers(1, &block.id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, block.id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, block.size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Bind buffer to binding point
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, block.bindingPoint, block.id);
}

void ShaderStorageBuffer::setValue(std::string_view blockName, std::string_view member, bool value)
{
	setValue(blockName, member, static_cast<uint32_t>(value), sizeof(uint32_t));
}

void ShaderStorageBuffer::setValue(std::string_view blockName, std::string_view member, glm::mat2 value)
{
	setValue(blockName, member, static_cast<glm::mat4>(value), sizeof(glm::vec4) * 2);
}

void ShaderStorageBuffer::setValue(std::string_view blockName, std::string_view member, glm::mat3 value)
{
	setValue(blockName, member, static_cast<glm::mat4>(value), sizeof(glm::vec4) * 3);
}

} // namespace Inferno
