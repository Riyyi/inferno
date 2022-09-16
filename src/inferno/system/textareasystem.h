#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <cstdint>  // std::uint32_t
#include <memory>   // std::shared_ptr
#include <optional> // std::optional
#include <vector>   // std::vector

#include "glm/ext/vector_float3.hpp" // glm::vec3

#include "inferno/render/renderer.h"
#include "inferno/singleton.h"

namespace Inferno {

	using CharacterQuad = std::array<CharacterVertex, Renderer::vertexPerQuad>;

	class Font;
	class Scene;

	class TextAreaSystem final : public Singleton<TextAreaSystem> {
	public:
		TextAreaSystem(s);
		virtual ~TextAreaSystem();

		void render();

		void setScene(Scene* scene) { m_scene = scene; }

	private:
		std::optional<CharacterQuad> calculateCharacterQuad(unsigned char character, std::shared_ptr<Font> font, float& advance);

		Scene* m_scene { nullptr };
	};

} // namespace Inferno


#endif // TEXTAREA_H
