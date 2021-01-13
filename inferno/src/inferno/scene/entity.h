#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint> // uint32_t, size_t
#include <memory>  // std::shared_ptr, std::weak_ptr
#include <utility> // std::forward

#include "entt/entity/registry.hpp" // ent::entity, entt::registry

#include "inferno/assertions.h"

namespace Inferno {

	class Entity {
	public:
		Entity(const std::shared_ptr<entt::registry>& registry);
		Entity(const std::shared_ptr<entt::registry>& registry, entt::entity handle);
		Entity(const std::shared_ptr<entt::registry>& registry, uint32_t handle);

		void valid() const;

		template<typename... T>
		[[nodiscard]] bool has() const
		{
			valid();
			return m_registry.lock()->has<T...>(m_entity);
		}

		template<typename... T>
		[[nodiscard]] bool any() const
		{
			valid();
			return m_registry.lock()->any<T...>(m_entity);
		}

		template<typename T, typename... P>
		T& add(P&&... parameters) const
		{
			valid();
			return m_registry.lock()->emplace_or_replace<T>(m_entity, std::forward<P>(parameters)...);
		};

		template<typename T>
		size_t remove() const
		{
			valid();
			return m_registry.lock()->remove_if_exists<T>(m_entity);
		}

		template<typename T, typename... P>
		T& get(P&&... parameters) const
		{
			valid();
			return m_registry.lock()->get_or_emplace<T>(m_entity, std::forward<P>(parameters)...);
		}

		// Casts
		inline operator bool() const { return m_entity != entt::null; }
		inline operator entt::entity() const { return m_entity; }
		inline operator uint32_t() const { return static_cast<uint32_t>(m_entity); }
		// Functor()
		inline uint32_t operator ()() const { return static_cast<uint32_t>(m_entity); }

	private:
		entt::entity m_entity = entt::null;

		std::weak_ptr<entt::registry> m_registry;
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, entt::entity handle);

}

#endif // ENTITY_H
