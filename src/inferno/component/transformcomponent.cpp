#include "inferno/component/transformcomponent.h"

namespace Inferno {

const LogStream& operator<<(const LogStream& stream, const glm::vec2& value)
{
	return stream << "{ "
	              << (value.x >= 0.0f ? " " : "") << value.x << ", "
	              << (value.y >= 0.0f ? " " : "") << value.y
	              << " }";
}

const LogStream& operator<<(const LogStream& stream, const glm::vec3& value)
{
	return stream << "{ "
	              << (value.x >= 0.0f ? " " : "") << value.x << ", "
	              << (value.y >= 0.0f ? " " : "") << value.y << ", "
	              << (value.z >= 0.0f ? " " : "") << value.z
	              << " }";
}

const LogStream& operator<<(const LogStream& stream, const glm::vec4& value)
{
	return stream << "{ "
	              << (value.x >= 0.0f ? " " : "") << value.x << ", "
	              << (value.y >= 0.0f ? " " : "") << value.y << ", "
	              << (value.z >= 0.0f ? " " : "") << value.z << ", "
	              << (value.w >= 0.0f ? " " : "") << value.w
	              << " }";
}

const LogStream& operator<<(const LogStream& stream, const glm::mat4& value)
{
	return stream << "mat4 "
	              << value[0] << "\n     " << value[1] << "\n     "
	              << value[2] << "\n     " << value[3];
}

const LogStream& operator<<(const LogStream& stream, const TransformComponent& value)
{
	return stream << "transform "
	              << value.translate << " t\n          "
	              << value.rotate << " r\n          "
	              << value.scale << " s";
}

} // namespace Inferno
