#include "inferno/log.h"
#include "inferno/scene/components.h"
#include "inferno/scene/entity.h"
#include "inferno/scene/scene.h"

namespace Inferno {

	Scene::Scene()
	{
		m_registry = std::make_shared<entt::registry>();

		Entity entity = createEntity("Test Entity");

		dbg() << entity.get<TagComponent>();

		if (entity) {
			dbg() << "Entity is valid";
		}
	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = Entity(m_registry);
		entity.add<TagComponent>(name.empty() ? "Unnamed Entity" : name);
		entity.add<TransformComponent>();

		return entity;
	}

	Entity Scene::createEntity(entt::entity handle)
	{
		return Entity(m_registry, handle);
	}

	Entity Scene::createEntity(uint32_t handle)
	{
		return Entity(m_registry, handle);
	}

}
