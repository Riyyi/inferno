#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <memory> // std::shared_ptr

#include "glm/ext/vector_float4.hpp" // glm::vec4

#include "inferno/render/texture.h"

namespace Inferno {

	struct SpriteComponent {
		glm::vec4 color { 1.0f };
		std::shared_ptr<Texture> texture;
	};

}

#endif // SPRITE_COMPONENT_H
