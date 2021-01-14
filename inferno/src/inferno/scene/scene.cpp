#include "inferno/log.h"
#include "inferno/scene/components.h"
#include "inferno/scene/entity.h"
#include "inferno/scene/scene.h"
#include "inferno/systems/camera.h"
#include "inferno/systems/render.h"
#include "inferno/systems/transform.h"

namespace Inferno {

	void Scene::initialize()
	{
		// Initialize
		// ---------------------------------

		m_registry = std::make_shared<entt::registry>();

		TransformSystem* transformSystem = new TransformSystem();
		transformSystem->initialize();
		TransformSystem::the().setRegistry(m_registry);

		CameraSystem* cameraSystem = new CameraSystem();
		cameraSystem->initialize();
		CameraSystem::the().setRegistry(m_registry);

#if 1
		Entity camera = createEntity("Camera Entity");
		camera.add<PerspectiveCameraComponent>();
		auto& cameraTransform = camera.get<TransformComponent>();
		cameraTransform.translate.z = 1.0f;
#else
		Entity camera = createEntity("Camera Entity");
		camera.add<OrthographicCameraComponment>();
		auto& cameraTransform = camera.get<TransformComponent>();
		cameraTransform.translate.z = -1.0f;
#endif

		RenderSystem* renderSystem = new RenderSystem();
		renderSystem->initialize();
		RenderSystem::the().setRegistry(m_registry);

		// Load assets
		// ---------------------------------

		m_texture = TextureManager::the().load("assets/gfx/test.png");
		m_texture2 = TextureManager::the().load("assets/gfx/test-inverted.png");

		// Construct entities
		// ---------------------------------

		Entity quad = createEntity("Quad");
		quad.add<SpriteComponent>(glm::vec4 { 1.0f, 1.0f, 1.0f, 1.0f }, m_texture);

		Entity quad2 = createEntity("Quad 2");
		auto& quad2Transform = quad2.get<TransformComponent>();
		quad2Transform.translate.x = 1.1f;
		quad2.add<SpriteComponent>(glm::vec4 { 0.5f, 0.6f, 0.8f, 1.0f }, m_texture);

		Entity quad3 = createEntity("Quad 3");
		auto& quad3Transform = quad3.get<TransformComponent>();
		quad3Transform.translate.x = 2.2f;
		quad3.add<SpriteComponent>(glm::vec4 { 1.0f, 1.0f, 1.0f, 1.0f }, m_texture2);

		dbg(Log::Info) << "Scene initialized";
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
		return CameraSystem::the().cameraProjectionView();
	}

}
