/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>       // int32_t, uint32_t
#include <memory>        // std::shared_ptr
#include <string>        // std::string
#include <unordered_map> // std::unordered_map

#include "glm/glm.hpp"
#include "ruc/singleton.h"

namespace Inferno {

class Shader {
public:
	Shader(const std::string& name);
	virtual ~Shader();

	int32_t findUniform(const std::string& name) const;

	void setInt(const std::string& name, int value);
	void setInt(const std::string& name, int* values, uint32_t count);
	void setFloat(const std::string& name, float value) const;
	void setFloat(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setFloat(const std::string& name, glm::vec2 value) const;
	void setFloat(const std::string& name, glm::vec3 value) const;
	void setFloat(const std::string& name, glm::vec4 value) const;
	void setFloat(const std::string& name, glm::mat3 matrix) const;
	void setFloat(const std::string& name, glm::mat4 matrix) const;

	void bind() const;
	void unbind() const;

	inline std::string name() const { return m_name; }
	inline uint32_t id() const { return m_id; }

protected:
	uint32_t compileShader(int32_t type, const char* shaderSource) const;
	uint32_t linkShader(uint32_t vertex, uint32_t fragment) const;
	int32_t checkStatus(uint32_t check, bool isProgram = false) const;

private:
	std::string m_name;
	uint32_t m_id;
};

// -------------------------------------

class ShaderManager final : public ruc::Singleton<ShaderManager> {
public:
	ShaderManager(s);
	virtual ~ShaderManager();

	void add(const std::string& name, std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> load(const std::string& name);
	std::shared_ptr<Shader> load(const std::string& vertexSource,
	                             const std::string& fragmentSource);
	std::shared_ptr<Shader> get(const std::string& name);
	bool exists(const std::string& name);

	void remove(const std::string& name);
	void remove(std::shared_ptr<Shader> shader);

protected:
	std::string computeName(const std::string& vertexSource,
	                        const std::string& fragmentSource);

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderList;
};

} // namespace Inferno
