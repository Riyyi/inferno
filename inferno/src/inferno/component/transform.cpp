#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::radians

#include "inferno/component/transform.h"

namespace Inferno {

	void Transform::update()
	{
		// Identity matrix
		m_transform = glm::mat4(1.0f);

		// Translate
		m_transform = glm::translate(m_transform, m_translate);

		// Rotate
		m_transform = glm::rotate(m_transform, glm::radians(m_rotate.x), {1.0, 0.0, 0.0});
		m_transform = glm::rotate(m_transform, glm::radians(m_rotate.y), {0.0, 1.0, 0.0});
		m_transform = glm::rotate(m_transform, glm::radians(m_rotate.z), {0.0, 0.0, 1.0});

		// Scale
		m_transform = glm::scale(m_transform, m_scale);
	}

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const Transform& value)
	{
		stream << "transform "
		       << value.translate() << " t\n          "
		       << value.rotate() << " r\n          "
		       << value.scale() << " s";
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const glm::vec2& value)
	{
		stream << "{ " << value.x << ", " << value.y << ", " << " }";
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value)
	{
		stream << "{ " << value.x << ", " << value.y << ", " << value.z << " }";
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const glm::vec4& value)
	{
		stream << "{ " << value.x << ", " << value.y << ", " << value.z << ", " << value.w << " }";
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value)
	{
		stream << "mat4 "
		       << value[0] << "\n     " << value[1] << "\n     "
		       << value[2] << "\n     " << value[3];
		return stream;
	}

}
