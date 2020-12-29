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
		void setFloat(const std::string &name, float f1) const;
		void setFloat(const std::string &name, float f1, float f2, float f3, float f4) const;
		void setFloat(const std::string &name, glm::vec2 v) const;
		void setFloat(const std::string &name, glm::vec3 v) const;
		void setFloat(const std::string &name, glm::vec4 v) const;
		void setFloat(const std::string &name, glm::mat3 m) const;
		void setFloat(const std::string &name, glm::mat4 m) const;

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
