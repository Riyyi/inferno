#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string> // std::string

#include "glm/ext/vector_float3.hpp" // glm::vec3

namespace Inferno {

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator const std::string&() const { return tag; }
	};

	struct TransformComponent {
		glm::vec3 translate = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotate = { 0.0f, 0.0f, 0.0f } ;
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};

}

#endif // COMPONENTS_H
