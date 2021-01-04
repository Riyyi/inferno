#ifndef CAMERA_H
#define CAMERA_H

#include "inferno/log.h" // @@@@@@@@@@
#define TRANSLATE_SPEED 2.5f
#define ROTATE_SPEED 90.0f
#define SENSITIVITY 0.25f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#include <memory> // std::shared_ptr

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm:vec3

namespace Inferno {

	class Transform;

	class Camera {
	public:
		Camera();
		Camera(glm::vec3 translate, glm::vec3 rotate);
		virtual ~Camera() { dbg(Log::Danger) << "KILLED CAMERA"; }

		virtual void initialize() = 0;
		virtual void update(float deltaTime) = 0;
		// virtual void render() = 0;
		// virtual void destroy() = 0;

		void setProjection(glm::mat4 projection) { m_projection = projection; }

		const glm::mat4& projection() const { return m_projection; }
		const std::shared_ptr<Transform>& transform() const { return m_transform; }

	private:
		glm::mat4 m_projection;
		std::shared_ptr<Transform> m_transform;
	};

// ----------------------------------------

	class OrthographicCamera final : public Camera {
	public:
		OrthographicCamera();
		OrthographicCamera(glm::vec3 translate, glm::vec3 rotate);
		virtual ~OrthographicCamera() { dbg(Log::Danger) << "KILLED ORTHOGRAPHICCAMERA"; }

		virtual void initialize() override;
		virtual void update(float deltaTime) override;
		// virtual void render() override;
		// virtual void destroy() override;

	private:
		glm::vec3 m_rotate;
	};

// ----------------------------------------

	class PerspectiveCamera final : public Camera {
	public:
		PerspectiveCamera();
		PerspectiveCamera(glm::vec3 translate, glm::vec3 rotate);
		virtual ~PerspectiveCamera() { dbg(Log::Danger) << "KILLED PERSPECTIVECAMERA"; }

		virtual void initialize() override;
		virtual void update(float deltaTime) override;
		// virtual void render() override;
		// virtual void destroy() override;

	private:
		float m_fov;
		float m_pitch;
		float m_yaw;
		glm::vec3 m_up;
	};

// ----------------------------------------

}

#endif // CAMERA_H

// @Todo:
// - Add sensitivity, fov to settings:camera
// - Add zoom to ortho camera
// - Change ortho view matrix to use glm::lookAt()
