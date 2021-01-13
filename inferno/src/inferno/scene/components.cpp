#include "inferno/scene/components.h"

namespace Inferno {

	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value)
	{
		stream << "{ " << value.x << ", " << value.y << ", " << value.z << " }";
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value)
	{
		stream << "mat4 "
		       << value[0] << "\n     " << value[1] << "\n     "
		       << value[2] << "\n     " << value[3];
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const TransformComponent& value)
	{
		return stream << "transform "
		              << value.translate << " t\n          "
		              << value.rotate << " r\n          "
		              << value.scale << " s";
	}

}
