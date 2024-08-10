/*
 * Copyright (C) 2022-2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstddef> // size_t
#include <cstdint> // uint32_t
#include <limits>  // std::numeric_limits
#include <utility> // std::pair

#include "entt/entity/entity.hpp" // ent::entity
#include "ruc/file.h"
#include "ruc/format/log.h"
#include "ruc/json/json.h"
#include "ruc/meta/assert.h"

#include "inferno/component/cameracomponent.h"
#include "inferno/component/cubemap-component.h"
#include "inferno/component/id-component.h"
#include "inferno/component/luascriptcomponent.h"
#include "inferno/component/model-component.h"
#include "inferno/component/nativescriptcomponent.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/tagcomponent.h"
#include "inferno/component/textareacomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/render/uniformbuffer.h"
#include "inferno/scene/scene.h"
#include "inferno/script/nativescript.h"
#include "inferno/system/camerasystem.h"
#include "inferno/system/rendersystem.h"
#include "inferno/system/scriptsystem.h"
#include "inferno/system/textareasystem.h"
#include "inferno/system/transformsystem.h"
#include "inferno/uid.h"

namespace Inferno {

void Scene::initialize()
{
	// Initialize
	// ---------------------------------

	m_registry = std::make_shared<entt::registry>();

	TransformSystem::the().setRegistry(m_registry);
	CameraSystem::the().setRegistry(m_registry);
	RenderSystem::the().setRegistry(m_registry);
	ScriptSystem::the().setScene(this);
	TextAreaSystem::the().setScene(this);

	// Load scene .json
	// -------------------------------------

	auto sceneJson = ruc::Json::parse(ruc::File("assets/scene/scene1.json").data());

	if (sceneJson.exists("init")) {
		// TODO: load either NativeScript or LuaScript?
	}

	// Entities
	// -------------------------------------

	if (sceneJson.exists("entities")) {
		const auto& entityJson = sceneJson.at("entities");
		VERIFY(entityJson.type() == ruc::Json::Type::Array);
		const auto& entities = entityJson.asArray();
		for (size_t i = 0; i < entities.size(); ++i) {
			loadEntity(entities.at(i));
		}
	}

	ruc::info("Scene initialized");
}

void Scene::update(float deltaTime)
{
	ScriptSystem::the().update(deltaTime);

	TransformSystem::the().update();
	CameraSystem::the().update();
}

void Scene::render()
{
	auto [projection, view] = CameraSystem::the().projectionView();
	Uniformbuffer::the().setFloat("Camera", "u_projectionView", projection * view);
	RendererCubemap::the().beginScene(projection, view); // camera, lights, environment

	RenderSystem::the().render();
	TextAreaSystem::the().render();

	RendererCubemap::the().endScene();
	Renderer3D::the().endScene();
	Renderer2D::the().endScene();
	RendererFont::the().endScene();
}

void Scene::destroy()
{
	ScriptSystem::destroy();
	RenderSystem::destroy();
	CameraSystem::destroy();
	TransformSystem::destroy();
}

// -----------------------------------------

uint32_t Scene::createEntity(const std::string& name)
{
	return createEntityWithUID(UID(), name);
}

uint32_t Scene::createEntityWithUID(UID id, const std::string& name)
{
	uint32_t entity = static_cast<uint32_t>(m_registry->create());
	addComponent<IDComponent>(entity, id);
	addComponent<TagComponent>(entity, name.empty() ? "Unnamed Entity" : name);
	addComponent<TransformComponent>(entity);

	TransformSystem::the().add(entity);

	return entity;
}

uint32_t Scene::loadEntity(ruc::Json components, uint32_t parentEntity)
{
	VERIFY(components.type() == ruc::Json::Type::Object);

	uint32_t entity = createEntity();

	// At minimum, ID is required
	VERIFY(components.exists("id"), "id not found");
	auto& id = getComponent<IDComponent>(entity);
	components.at("id").getTo(id);

	if (components.exists("tag")) {
		auto& tag = getComponent<TagComponent>(entity);
		components.at("tag").getTo(tag);
	}
	if (components.exists("transform")) {
		auto& transform = getComponent<TransformComponent>(entity);
		components.at("transform").getTo(transform);
		transform.parent = static_cast<entt::entity>(parentEntity);
	}
	if (components.exists("camera")) {
		auto& camera = addComponent<CameraComponent>(entity);
		components.at("camera").getTo(camera);
	}
	if (components.exists("lua-scripts")) {
		VERIFY(components.at("lua-scripts").type() == ruc::Json::Type::Array);
		const auto& scripts = components.at("lua-scripts").asArray();
		for (size_t i = 0; i < scripts.size(); ++i) {
			auto& script = addComponent<LuaScriptComponent>(entity);
			scripts.at(i).getTo(script);
		}
	}
	if (components.exists("native-scripts")) {
		VERIFY(components.at("native-scripts").type() == ruc::Json::Type::Array);
		const auto& scripts = components.at("native-scripts").asArray();
		for (size_t i = 0; i < scripts.size(); ++i) {
			auto& script = addComponent<NativeScriptComponent>(entity);
			scripts.at(i).getTo(script);
			script.bind();
		}
	}
	if (components.exists("sprite")) {
		auto& sprite = addComponent<SpriteComponent>(entity);
		components.at("sprite").getTo(sprite);
	}
	if (components.exists("cubemap")) {
		auto& cubemap = addComponent<CubemapComponent>(entity);
		components.at("cubemap").getTo(cubemap);
	}
	if (components.exists("text")) {
		auto& text = addComponent<TextAreaComponent>(entity);
		components.at("text").getTo(text);
	}
	if (components.exists("model")) {
		auto& text = addComponent<ModelComponent>(entity);
		components.at("model").getTo(text);
	}
	if (components.exists("children")) {
		VERIFY(components.at("children").type() == ruc::Json::Type::Array);
		const auto& children = components.at("children").asArray();
		for (size_t i = 0; i < children.size(); ++i) {
			loadEntity(components.at("children")[i], entity);
		}
	}

	return entity;
}

uint32_t Scene::findEntity(std::string_view name)
{
	auto view = m_registry->view<TagComponent>();

	for (auto [entity, tag] : view.each()) {
		if (tag.tag == name) {
			return static_cast<uint32_t>(entity);
		}
	}

	return std::numeric_limits<uint32_t>::max();
}

void Scene::destroyEntity(uint32_t entity)
{
	ScriptSystem::the().cleanup(entity);
	m_registry->destroy(entt::entity { entity });
}

// -----------------------------------------

std::pair<glm::mat4, glm::mat4> Scene::cameraProjectionView()
{
	return CameraSystem::the().projectionView();
}

void Scene::validEntity(uint32_t entity) const
{
	VERIFY(m_registry->valid(entt::entity { entity }), "invalid entity '{}'", entity);
}

} // namespace Inferno

void ruc::format::Formatter<entt::entity>::format(Builder& builder, entt::entity value) const
{
	return Formatter<uint32_t>::format(builder, static_cast<uint32_t>(value));
}
