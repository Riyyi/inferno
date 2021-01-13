#include "inferno/scene/entity.h"

namespace Inferno {

	Entity::Entity(const std::shared_ptr<entt::registry>& registry)
		: m_registry(registry)
	{
		expired();
		m_entity = m_registry.lock()->create();
	}

	Entity::Entity(const std::shared_ptr<entt::registry>& registry, entt::entity handle)
		: m_registry(registry)
	{
		expired();
		ASSERT(m_registry.lock()->valid(handle), "Can't construct entity from invalid handle");
		m_entity = handle;
	}

	Entity::Entity(const std::shared_ptr<entt::registry>& registry, uint32_t handle)
		: m_registry(registry)
	{
		expired();
		ASSERT(m_registry.lock()->valid(entt::entity(handle)), "Can't construct entity from invalid handle");
		m_entity = entt::entity(handle);
	}

	void Entity::expired() const
	{
		ASSERT(!m_registry.expired(), "Entity registry expired");
	}

	void Entity::valid() const
	{
		expired();
		ASSERT(m_registry.lock()->valid(m_entity), "Entity is not valid");
	}

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, entt::entity entity)
	{
		return stream << static_cast<uint32_t>(entity);
	}

}
