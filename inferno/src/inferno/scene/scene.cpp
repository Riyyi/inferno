#include "inferno/log.h"
#include "inferno/scene/components.h"
#include "inferno/scene/entity.h"
#include "inferno/scene/scene.h"
#include "inferno/systems/camera.h"
#include "inferno/systems/render.h"
#include "inferno/systems/transform.h"

namespace Inferno {

	Scene::Scene()
	{
		initialize();
	}

	Scene::~Scene()
	{
		destroy();
	}

	void Scene::initialize()
	{
		m_registry = std::make_shared<entt::registry>();

		TransformSystem* transformSystem = new TransformSystem();
		transformSystem->initialize();
		TransformSystem::the().setRegistry(m_registry);

		CameraSystem* cameraSystem = new CameraSystem();
		cameraSystem->initialize();
		CameraSystem::the().setRegistry(m_registry);

		Entity camera = createEntity("Camera Entity");
		camera.add<PerspectiveCameraComponent>();
		auto& cameraTransform = camera.get<TransformComponent>();
		cameraTransform.translate.z = 1.0f;
		cameraTransform.rotate.z = -1.0f;

		RenderSystem* renderSystem = new RenderSystem();
		renderSystem->initialize();
		RenderSystem::the().setRegistry(m_registry);
	}

	void Scene::update(float deltaTime)
	{
		(void)deltaTime;

		TransformSystem::the().update();
		CameraSystem::the().update();
	}

	void Scene::render()
	{
		RenderSystem::the().render();
	}

	void Scene::destroy()
	{
		RenderSystem::the().destroy();
		CameraSystem::the().destroy();
		TransformSystem::the().destroy();
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

	glm::mat4 Scene::cameraProjectionView()
	{
		auto view = m_registry->view<TransformComponent, PerspectiveCameraComponent>();

		for(auto&& [entity, transform, perspective] : view.each()) {
			return perspective.projection * transform.transform;
		}

		return glm::mat4 { 1.0f };
	}

}
