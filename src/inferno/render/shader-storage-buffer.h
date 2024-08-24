/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstddef> // size_t
#include <cstdint> // uint8_t, uint32_t
#include <string_view>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/ext/matrix_float2x2.hpp" // glm::mat2
#include "glm/ext/matrix_float3x3.hpp" // glm::mat3
#include "ruc/singleton.h"

#define CHECK_SET_CALL(blockName, member)                                                 \
	VERIFY(exists(blockName), "shader storage buffer block doesnt exist: {}", blockName); \
	const BufferBlock& block = m_blocks[blockName.data()];                                \
	VERIFY(block.memberOffsets.find(member.data()) != block.memberOffsets.end(),          \
	       "shader storage buffer member doesnt exist: {}", member);

namespace Inferno {

struct BufferBlock {
	uint32_t id { 0 };
	uint32_t size { 0 };
	uint8_t bindingPoint { 0 };
	std::unordered_map<std::string, uint32_t> memberOffsets {};
};

class ShaderStorageBuffer final : public ruc::Singleton<ShaderStorageBuffer> { // Shader Storage Buffer Object, SSBO
public:
	ShaderStorageBuffer(s);
	virtual ~ShaderStorageBuffer();

	void setLayout(std::string_view blockName, uint8_t bindingPoint, uint32_t shaderID);
	void create(std::string_view blockName);

	bool exists(std::string_view blockName) const { return m_blocks.find(blockName.data()) != m_blocks.end(); }

	template<typename T> // Capture value by reference, instead of decaying to pointer
	void setValue(std::string_view blockName, std::string_view member, T&& value, size_t size = 0)
	{
		CHECK_SET_CALL(blockName, member);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, block.id);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, block.memberOffsets.at(member.data()), (size) ? size : sizeof(T), &value);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	// Exceptions:
	void setValue(std::string_view blockName, std::string_view member, bool value);
	void setValue(std::string_view blockName, std::string_view member, glm::mat2 value);
	void setValue(std::string_view blockName, std::string_view member, glm::mat3 value);

private:
	uint8_t m_maxBindingPoints { 0 };
	std::unordered_map<std::string, BufferBlock> m_blocks;
};

} // namespace Inferno
