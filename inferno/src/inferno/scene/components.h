#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <memory>     // std::shared_ptr
#include <string>     // std::string
#include <functional> // std::function

#include "glm/ext/matrix_float4x4.hpp" // glm::mat4
#include "glm/ext/vector_float3.hpp"   // glm::vec3

#include "inferno/assertions.h"
#include "inferno/log.h"
#include "inferno/render/texture.h"
#include "inferno/script/nativescript.h"

namespace Inferno {

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator const std::string&() const { return tag; }
	};

	struct TransformComponent {
		glm::vec3 translate { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotate { 0.0f, 0.0f, 0.0f } ;
		glm::vec3 scale { 1.0f, 1.0f, 1.0f };
		glm::mat4 transform { 1.0f }; // Identity matrix
	};

	enum CameraType {
		Orthographic,
		Perspective,
	};

	struct CameraComponent {
		CameraType type = CameraType::Perspective;

		// Orthographic
		float zoomLevel = 1.0f;
		glm::vec3 rotateAxis { 0.0f, 0.0f, 1.0f };

		// Perspective
		float fov = 90.0f;
		float pitch = 0.0f;
		float yaw = -90.0f;
		glm::vec3 up { 0.0f, 1.0f, 0.0f };

		glm::mat4 projection { 1.0f }; // Identity matrix
	};

	struct SpriteComponent {
		glm::vec4 color { 1.0f };
		std::shared_ptr<Texture> texture;
	};

	struct NativeScriptComponent {
		NativeScript* instance = nullptr;

		std::function<void()> initialize = nullptr;
		std::function<void()> destroy = nullptr;

		// Dont allow manually setting instance during construction
		NativeScriptComponent() {}

		template<typename T>
		void bind()
		{
			ASSERT(initialize == nullptr && destroy == nullptr, "NativeScript already bound");
			initialize = [&]() { instance = static_cast<NativeScript*>(new T()); };
			destroy = [&]() { delete instance; instance = nullptr; initialize = nullptr; destroy = nullptr; };
		}
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const TransformComponent& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec3& value);
	const LogStream& operator<<(const LogStream& stream, const glm::vec4& value);
	const LogStream& operator<<(const LogStream& stream, const glm::mat4& value);

}

#endif // COMPONENTS_H
