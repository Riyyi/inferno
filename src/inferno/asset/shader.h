/*
 * Copyright (C) 2022,2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t, uint32_t
#include <string_view>
#include <unordered_map>

#include "glm/fwd.hpp" // glm::mat3, glm::mat4, glm::vec2, glm::vec3

#include "inferno/asset/asset-manager.h"

namespace Inferno {

class Shader : public Asset {
public:
	virtual ~Shader();

	// Factory function
	static std::shared_ptr<Shader> create(std::string_view path);

	int32_t findUniformLocation(std::string_view name);

	void setInt(std::string_view name, int value);
	void setInt(std::string_view name, int* values, uint32_t count);
	void setFloat(std::string_view name, float value);
	void setFloat(std::string_view name, float v1, float v2, float v3, float v4);
	void setFloat(std::string_view name, glm::vec2 value);
	void setFloat(std::string_view name, glm::vec3 value);
	void setFloat(std::string_view name, glm::vec4 value);
	void setFloat(std::string_view name, glm::mat3 matrix);
	void setFloat(std::string_view name, glm::mat4 matrix);

	void bind() const;
	void unbind() const;

	uint32_t id() const { return m_id; }

protected:
	uint32_t compileShader(int32_t type, const char* shaderSource) const;
	uint32_t linkShader(uint32_t vertex, uint32_t fragment) const;
	int32_t checkStatus(uint32_t check, bool isProgram = false) const;

private:
	Shader(std::string_view path)
		: Asset(path)
	{
	}

	virtual bool isShader() const override { return true; }

private:
	uint32_t m_id { 0 };
	std::unordered_map<std::string_view, int32_t> m_uniformLocation;
};

// -----------------------------------------

// clang-format off
template<>
inline bool Asset::fastIs<Shader>() const { return isShader(); }
// clang-format on

} // namespace Inferno
