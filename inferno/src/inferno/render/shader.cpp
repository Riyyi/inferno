#include <vector> // std::vector

#include <glad/glad.h>

#include "inferno/core.h"
#include "inferno/file.h"
#include "inferno/log.h"
#include "inferno/render/shader.h"

namespace Inferno {

	Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) :
		m_program(0)
	{
		// Get file contents
		std::string vertexSrc = File::read(vertexSource);
		std::string fragmentSrc = File::read(fragmentSource);

		// Compile shaders
		uint32_t vertexID = compileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
		uint32_t fragmentID = compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

		// Link shaders
		if (vertexID > 0 && fragmentID > 0) {
			m_program = linkShader(vertexID, fragmentID);
		}
		// Clear resources
		else if (vertexID > 0)   glDeleteShader(vertexID);
		else if (fragmentID > 0) glDeleteShader(fragmentID);
	}

	Shader::~Shader()
	{
		if (m_program > 0) {
			glDeleteProgram(m_program);
			m_program = 0;
		}
	}

// -----------------------------------------

	void Shader::bind() const
	{
		glUseProgram(m_program);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

// -----------------------------------------

	uint32_t Shader::getProgram() const
	{
		return m_program;
	}

// -----------------------------------------

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

			NF_CORE_WARN("Shader %s", infoLog.data());
		}

		NF_CORE_ASSERT(success == GL_TRUE, "Shader program creation failed!")

		return success;
	}

}