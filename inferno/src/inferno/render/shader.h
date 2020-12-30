#ifndef SHADER_H
#define SHADER_H

#include <cstdint> // std::int32_t, std::uint32_t
#include <string>  // std::string

#include <glm/glm.hpp>

namespace Inferno {

	class Shader {
	public:
		Shader(const std::string &vertexSource, const std::string &fragmentSource);
		~Shader();

// -----------------------------------------

		// Set uniform data
		void setInt(const std::string& name, int value);
		void setFloat(const std::string &name, float value) const;
		void setFloat(const std::string &name, float v1, float v2, float v3, float v4) const;
		void setFloat(const std::string &name, glm::vec2 value) const;
		void setFloat(const std::string &name, glm::vec3 value) const;
		void setFloat(const std::string &name, glm::vec4 value) const;
		void setFloat(const std::string &name, glm::mat3 matrix) const;
		void setFloat(const std::string &name, glm::mat4 matrix) const;

// -----------------------------------------

		void bind() const;
		void unbind() const;

// -----------------------------------------

		inline uint32_t getProgram() const { return m_program; }

// -----------------------------------------

	protected:
		uint32_t compileShader(int32_t type, const char* shaderSource) const;
		uint32_t linkShader(uint32_t vertex, uint32_t fragment) const;
		int32_t checkStatus(uint32_t check, bool isProgram = false) const;

	private:
		uint32_t m_program;
	};

}


#endif // SHADER_H
