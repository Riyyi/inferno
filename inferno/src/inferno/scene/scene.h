#ifndef SCENE_H
#define SCENE_H

#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr

#include "entt/entity/registry.hpp"    // entt::entity, entt::registry
#include "glm/ext/matrix_float4x4.hpp" // glm::mat4

#include "inferno/log.h"

namespace Inferno {

	class Camera;
	class Entity;
	class Texture;

	class Scene {
	public:
		void initialize();
		void update(float deltaTime);
		void render();
		void destroy();

		uint32_t createEntity(const std::string& name = "");
		void destroyEntity(uint32_t entity);

		glm::mat4 cameraProjectionView();

		void validEntity(uint32_t entity) const;

		template<typename... T>
		[[nodiscard]] bool hasComponent(uint32_t entity) const
		{
			validEntity(entity);
			return m_registry->has<T...>(entt::entity { entity });
		}

		template<typename... T>
		[[nodiscard]] bool anyComponent(uint32_t entity) const
		{
			validEntity(entity);
			return m_registry->any<T...>(entt::entity { entity });
		}

		// @Todo Should replace be allowed? could trigger memory leaks with nativescript
		template<typename T, typename... P>
		T& addComponent(uint32_t entity, P&&... parameters) const
		{
			validEntity(entity);
			return m_registry->emplace_or_replace<T>(entt::entity { entity }, std::forward<P>(parameters)...);
		};

		template<typename T>
		size_t removeComponent(uint32_t entity) const
		{
			validEntity(entity);
			return m_registry->remove_if_exists<T>(entt::entity { entity });
		}

		// @Todo Should replace be allowed? could trigger memory leaks with nativescript
		template<typename T, typename... P>
		T& getComponent(uint32_t entity, P&&... parameters) const
		{
			validEntity(entity);
			return m_registry->get_or_emplace<T>(entt::entity { entity }, std::forward<P>(parameters)...);
		}

		// const entt::registry& registry() const { return m_registry; }
		std::shared_ptr<entt::registry> registry() const { return m_registry; }

	private:
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Texture> m_texture2;

		std::shared_ptr<entt::registry> m_registry;
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, entt::entity handle);

}

#endif // SCENE_H
