#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/ext/matrix_float4x4.hpp> // glm::mat4
#include <glm/ext/vector_float3.hpp>   // glm::vec3

#include "inferno/log.h"

namespace Inferno {

	class Transform {
	public:
		Transform() {}
		Transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
			: m_translate(translate), m_rotate(rotate), m_scale(scale) {}
		virtual ~Transform() { dbg(Log::Danger) << "KILLED TRANSFORM"; }

		virtual void update();

		void setTranslate(glm::vec3 translate) { m_translate = translate; }
		void setRotate(glm::vec3 rotation) { m_rotate = rotation; }
		void setScale(glm::vec3 scale) { m_scale = scale; }
		void setTransform(glm::mat4 transform) { m_transform = transform; }

		void setTranslate(float x, float y, float z) { m_translate = { x, y, z }; }
		void setRotate(float x, float y, float z) { m_rotate = { x, y, z }; }
		void setScale(float x, float y, float z) { m_scale = { x, y, z }; }

		const glm::vec3& translate() const { return m_translate; }
		const glm::vec3& rotate() const { return m_rotate; }
		const glm::vec3& scale() const { return m_scale; }
		const glm::mat4& transform() const { return m_transform; }

	private:
		glm::vec3 m_translate;
		glm::vec3 m_rotate;
		glm::vec3 m_scale;
		glm::mat4 m_transform;
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const Transform& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec4& value);
	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value);

}

#endif // TRANSFORM_H
