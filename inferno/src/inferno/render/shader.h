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

		void bind() const;
		void unbind() const;

// -----------------------------------------

		uint32_t getProgram() const;

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
