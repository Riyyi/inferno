#include <utility> // std::move
#include <vector>  // std::vector

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr
#include "ruc/meta/assert.h"

#include "inferno/core.h"
#include "inferno/io/file.h"
#include "inferno/io/log.h"
#include "inferno/render/shader.h"

namespace Inferno {

Shader::Shader(const std::string& name)
	: m_name(std::move(name))
	, m_id(0)
{
	// Get file contents
	std::string vertexSrc = File::read(name + ".vert");
	std::string fragmentSrc = File::read(name + ".frag");

	// Compile shaders
	uint32_t vertexID = compileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	uint32_t fragmentID = compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	// Link shaders
	if (vertexID > 0 && fragmentID > 0) {
		m_id = linkShader(vertexID, fragmentID);
	}
	// Clear resources
	else if (vertexID > 0)
		glDeleteShader(vertexID);
	else if (fragmentID > 0)
		glDeleteShader(fragmentID);
}

Shader::~Shader()
{
	if (m_id > 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

int32_t Shader::findUniform(const std::string& name) const
{
	int32_t location = glGetUniformLocation(m_id, name.c_str());
	VERIFY(location != -1, "Shader could not find uniform '{}'", name);
	return location;
}

void Shader::setInt(const std::string& name, int value)
{
	// Set uniform int
	glUniform1i(findUniform(name), value);
}

void Shader::setInt(const std::string& name, int* values, uint32_t count)
{
	// Set uniform int array
	glUniform1iv(findUniform(name), count, values);
}

void Shader::setFloat(const std::string& name, float value) const
{
	// Set uniform float
	glUniform1f(findUniform(name), value);
}

void Shader::setFloat(const std::string& name, float v1, float v2, float v3, float v4) const
{
	// Set uniform vec4 data
	glUniform4f(findUniform(name), v1, v2, v3, v4);
}

void Shader::setFloat(const std::string& name, glm::vec2 value) const
{
	// Set uniform vec2 data
	glUniform2f(findUniform(name), value.x, value.y);
}

void Shader::setFloat(const std::string& name, glm::vec3 value) const
{
	// Set uniform vec3 data
	glUniform3f(findUniform(name), value.x, value.y, value.z);
}

void Shader::setFloat(const std::string& name, glm::vec4 value) const
{
	// Set uniform vec4 data
	glUniform4f(findUniform(name), value.x, value.y, value.z, value.w);
}

void Shader::setFloat(const std::string& name, glm::mat3 matrix) const
{
	// Set uniform mat3 data
	glUniformMatrix3fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setFloat(const std::string& name, glm::mat4 matrix) const
{
	// Set uniform mat4 data
	glUniformMatrix4fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
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

		warn() << "Shader " << infoLog.data();
	}

	VERIFY(success == GL_TRUE, "Shader program creation failed!");

	return success;
}

// -----------------------------------------

ShaderManager::ShaderManager(s)
{
	info() << "ShaderManager initialized";
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::add(const std::string& name, std::shared_ptr<Shader> shader)
{
	// Construct (key, value) pair and insert it into the unordered_map
	m_shaderList.emplace(std::move(name), std::move(shader));
}

std::shared_ptr<Shader> ShaderManager::load(const std::string& name)
{
	if (exists(name)) {
		return get(name);
	}

	std::shared_ptr<Shader> shader = std::make_shared<Shader>(name);
	add(name, shader);
	return get(name);
}

std::shared_ptr<Shader> ShaderManager::load(const std::string& vertexSource,
                                            const std::string& fragmentSource)
{
	std::string name = computeName(vertexSource, fragmentSource);
	return load(name);
}

std::shared_ptr<Shader> ShaderManager::get(const std::string& name)
{
	return exists(name) ? m_shaderList.at(name) : nullptr;
}

bool ShaderManager::exists(const std::string& name)
{
	return m_shaderList.find(name) != m_shaderList.end();
}

void ShaderManager::remove(const std::string& name)
{
	if (exists(name)) {
		m_shaderList.erase(name);
	}
}

void ShaderManager::remove(std::shared_ptr<Shader> shader)
{
	if (exists(shader->name())) {
		m_shaderList.erase(shader->name());
	}
}

std::string ShaderManager::computeName(const std::string& vertexSource,
                                       const std::string& fragmentSource)
{
	auto vertexPos = vertexSource.find_last_of('.');
	auto fragmentPos = fragmentSource.find_last_of('.');

	VERIFY(vertexPos != std::string::npos, "Shader did not have file extension: '{}'", vertexSource);
	VERIFY(fragmentPos != std::string::npos, "Shader did not have file extension: '{}'", fragmentSource);

	auto vertexName = vertexSource.substr(0, vertexPos);
	auto fragmentName = vertexSource.substr(0, fragmentPos);

	VERIFY(vertexName == fragmentName, "Shader names did not match: {} {}", vertexSource, fragmentSource);

	return vertexName;
}

} // namespace Inferno
