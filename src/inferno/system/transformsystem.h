#pragma once

#include <memory> // std::shared_ptr

#include "entt/entity/registry.hpp" // entt::entity, entt::registry
#include "ruc/singleton.h"

namespace Inferno {

class TransformSystem final : public ruc::Singleton<TransformSystem> {
public:
	TransformSystem(s);
	virtual ~TransformSystem();

	void update();

	void setRegistry(std::shared_ptr<entt::registry> registry) { m_registry = registry; };

private:
	std::shared_ptr<entt::registry> m_registry;
};

} // namespace Inferno
