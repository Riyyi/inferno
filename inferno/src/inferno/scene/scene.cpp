#include "inferno/assert.h"
#include "inferno/component/cameracomponent.h"
#include "inferno/component/luascriptcomponent.h"
#include "inferno/component/nativescriptcomponent.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/tagcomponent.h"
#include "inferno/scene/scene.h"
#include "inferno/script/cameracontroller.h"
#include "inferno/script/nativescript.h"
#include "inferno/system/camerasystem.h"
#include "inferno/system/rendersystem.h"
#include "inferno/system/scriptsystem.h"
#include "inferno/system/transformsystem.h"

namespace Inferno {

	void Scene::initialize()
	{
		// Initialize
		// ---------------------------------

		m_registry = std::make_shared<entt::registry>();

		TransformSystem::initialize();
		TransformSystem::the().setRegistry(m_registry);

		CameraSystem::initialize();
		CameraSystem::the().setRegistry(m_registry);

		RenderSystem::initialize();
		RenderSystem::the().setRegistry(m_registry);

		ScriptSystem::initialize();
		ScriptSystem::the().setScene(this);

		// Load assets
		// ---------------------------------

		m_texture = TextureManager::the().load("assets/gfx/test.png");
		m_texture2 = TextureManager::the().load("assets/gfx/test-inverted.png");

		// Construct entities
		// ---------------------------------

		uint32_t camera = createEntity("Camera Entity");
		auto& cameraTransform = getComponent<TransformComponent>(camera);
		cameraTransform.rotate.z = -1.0f;
		cameraTransform.translate.z = 1.0f;
		addComponent<CameraComponent>(camera, CameraType::Perspective);
		// addComponent<NativeScriptComponent>(camera).bind<CameraController>();
		addComponent<LuaScriptComponent>(camera, "assets/lua/cameracontroller.lua");

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

		info() << "Scene initialized";
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
		ScriptSystem::destroy();
		RenderSystem::destroy();
		CameraSystem::destroy();
		TransformSystem::destroy();
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
		ScriptSystem::the().cleanup(entity);
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
