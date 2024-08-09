/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <vector> // std::vector

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr
#include "ruc/file.h"
#include "ruc/format/log.h"
#include "ruc/meta/assert.h"

#include "inferno/asset/shader.h"

namespace Inferno {

std::shared_ptr<Shader> Shader::create(std::string_view path)
{
	auto result = std::shared_ptr<Shader>(new Shader(path));

	// Get file contents
	auto stringPath = std::string(path);
	std::string vertexSrc = ruc::File(stringPath + ".vert").data();
	std::string fragmentSrc = ruc::File(stringPath + ".frag").data();

	// Compile shaders
	uint32_t vertexID = result->compileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	uint32_t fragmentID = result->compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	// Link shaders
	if (vertexID > 0 && fragmentID > 0) {
		result->m_id = result->linkShader(vertexID, fragmentID);
	}
	// Clear resources
	else if (vertexID > 0)
		glDeleteShader(vertexID);
	else if (fragmentID > 0)
		glDeleteShader(fragmentID);

	return result;
}

Shader::~Shader()
{
	if (m_id > 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

int32_t Shader::findUniformLocation(std::string_view name)
{
	// Cache uniform locations, prevent going to the GPU every call
	if (m_uniformLocation.find(name) != m_uniformLocation.end()) {
		return m_uniformLocation[name];
	}

	int32_t location = glGetUniformLocation(m_id, name.data());
	VERIFY(location != -1, "Shader could not find uniform '{}'", name);
	m_uniformLocation[name] = location;

	return location;
}

void Shader::setInt(std::string_view name, int value)
{
	// Set uniform int
	glUniform1i(findUniformLocation(name), value);
}

void Shader::setInt(std::string_view name, int* values, uint32_t count)
{
	// Set uniform int array
	glUniform1iv(findUniformLocation(name), count, values);
}

void Shader::setFloat(std::string_view name, float value)
{
	// Set uniform float
	glUniform1f(findUniformLocation(name), value);
}

void Shader::setFloat(std::string_view name, float v1, float v2, float v3, float v4)
{
	// Set uniform vec4 data
	glUniform4f(findUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setFloat(std::string_view name, glm::vec2 value)
{
	// Set uniform vec2 data
	glUniform2f(findUniformLocation(name), value.x, value.y);
}

void Shader::setFloat(std::string_view name, glm::vec3 value)
{
	// Set uniform vec3 data
	glUniform3f(findUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setFloat(std::string_view name, glm::vec4 value)
{
	// Set uniform vec4 data
	glUniform4f(findUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setFloat(std::string_view name, glm::mat3 matrix)
{
	// Set uniform mat3 data
	glUniformMatrix3fv(findUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setFloat(std::string_view name, glm::mat4 matrix)
{
	// Set uniform mat4 data
	glUniformMatrix4fv(findUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::bind() const
{
	glUseProgram(m_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

uint32_t Shader::compileShader(int32_t type, const char* source) const
{
	// Create new shader
	uint32_t shader = 0;
	shader = glCreateShader(type);
	// Attach shader source to shader object
	glShaderSource(shader, 1, &source, nullptr);
	// Compile shader
	glCompileShader(shader);

	// Check compilation status
	if (checkStatus(shader) == GL_TRUE) {
		return shader;
	}

	// On fail
	glDeleteShader(shader);
	return 0;
}

uint32_t Shader::linkShader(uint32_t vertex, uint32_t fragment) const
{
	// Create new shader program
	uint32_t shaderProgram = 0;
	shaderProgram = glCreateProgram();
	// Attach both shaders to the shader program
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	// Setup vertex attributes
	glBindAttribLocation(shaderProgram, 0, "a_position");
	// Link the shaders
	glLinkProgram(shaderProgram);
	// Clear resources
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Check linking status
	if (checkStatus(shaderProgram, true) == GL_TRUE) {
		return shaderProgram;
	}

	// On fail
	glDeleteProgram(shaderProgram);
	return 0;
}

int32_t Shader::checkStatus(uint32_t check, bool isProgram) const
{
	int32_t success;
	int32_t maxLength = 0;
	std::vector<char> infoLog;

	// Get the compilation/linking status
	!isProgram
		? glGetShaderiv(check, GL_COMPILE_STATUS, &success)
		: glGetProgramiv(check, GL_LINK_STATUS, &success);

	if (success != GL_TRUE) {
		// Get max length of the log including \0 terminator
		!isProgram
			? glGetShaderiv(check, GL_INFO_LOG_LENGTH, &maxLength)
			: glGetProgramiv(check, GL_INFO_LOG_LENGTH, &maxLength);

		// Reserve data for the log
		infoLog.reserve(maxLength);

		// Retrieve the error message
		!isProgram
			? glGetShaderInfoLog(check, maxLength, nullptr, &infoLog[0])
			: glGetProgramInfoLog(check, maxLength, nullptr, &infoLog[0]);

		ruc::warn("Shader {}", infoLog.data());
	}

	VERIFY(success == GL_TRUE, "Shader program creation failed!");

	return success;
}

} // namespace Inferno
