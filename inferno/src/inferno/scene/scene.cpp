#include "inferno/scene/components.h"
#include "inferno/scene/scene.h"
#include "inferno/script/cameracontroller.h"
#include "inferno/script/nativescript.h"
#include "inferno/systems/camera.h"
#include "inferno/systems/render.h"
#include "inferno/systems/script.h"
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

		uint32_t camera = createEntity("Camera Entity");
		auto& cameraTransform = getComponent<TransformComponent>(camera);
		cameraTransform.translate.z = 1.0f;
		addComponent<CameraComponent>(camera, CameraType::Perspective);
		addComponent<NativeScriptComponent>(camera).bind<CameraController>();

		RenderSystem* renderSystem = new RenderSystem();
		renderSystem->initialize();
		RenderSystem::the().setRegistry(m_registry);

		ScriptSystem* scriptSystem = new ScriptSystem();
		scriptSystem->initialize();
		ScriptSystem::the().setScene(this);

		// Load assets
		// ---------------------------------

		m_texture = TextureManager::the().load("assets/gfx/test.png");
		m_texture2 = TextureManager::the().load("assets/gfx/test-inverted.png");

		// Construct entities
		// ---------------------------------

		uint32_t quad = createEntity("Quad");
		addComponent<SpriteComponent>(quad, glm::vec4 { 1.0f, 1.0f, 1.0f, 1.0f }, m_texture);

		uint32_t quad2 = createEntity("Quad 2");
		auto& quad2Transform = getComponent<TransformComponent>(quad2);
		quad2Transform.translate.x = 1.1f;
		addComponent<SpriteComponent>(quad2, glm::vec4 { 0.5f, 0.6f, 0.8f, 1.0f }, m_texture);

		uint32_t quad3 = createEntity("Quad 3");
		auto& quad3Transform = getComponent<TransformComponent>(quad3);
		quad3Transform.translate.x = 2.2f;
		addComponent<SpriteComponent>(quad3, glm::vec4 { 1.0f, 1.0f, 1.0f, 1.0f }, m_texture2);

		dbg(Log::Info) << "Scene initialized";
	}

	void Scene::update(float deltaTime)
	{
		ScriptSystem::the().update(deltaTime);

		TransformSystem::the().update();
		CameraSystem::the().update();
	}

	void Scene::render()
	{
		RenderSystem::the().render();
	}

	void Scene::destroy()
	{
		ScriptSystem::the().destroy();
		RenderSystem::the().destroy();
		CameraSystem::the().destroy();
		TransformSystem::the().destroy();
	}

	uint32_t Scene::createEntity(const std::string& name)
	{
		uint32_t entity = static_cast<uint32_t>(m_registry->create());
		addComponent<TagComponent>(entity, name.empty() ? "Unnamed Entity" : name);
		addComponent<TransformComponent>(entity);

		return entity;
	}

	void Scene::destroyEntity(uint32_t entity)
	{
		if (hasComponent<NativeScript>(entity)) {
			ScriptSystem::the().cleanup(entity);
		}

		m_registry->destroy(entt::entity { entity });
	}

	glm::mat4 Scene::cameraProjectionView()
	{
		return CameraSystem::the().projectionView();
	}

	void Scene::validEntity(uint32_t entity) const
	{
		ASSERT(m_registry->valid(entt::entity {entity}), "Entity is not valid");
	}

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, entt::entity entity)
	{
		return stream << static_cast<uint32_t>(entity);
	}

}
