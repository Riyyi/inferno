/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstddef> // size_t
#include <cstdint> // uint32_t
#include <limits>  // std::numeric_limits

#include "ruc/file.h"
#include "ruc/format/log.h"
#include "ruc/json/json.h"
#include "ruc/meta/assert.h"

#include "inferno/component/cameracomponent.h"
#include "inferno/component/id-component.h"
#include "inferno/component/luascriptcomponent.h"
#include "inferno/component/nativescriptcomponent.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/tagcomponent.h"
#include "inferno/component/textareacomponent.h"
#include "inferno/component/transformcomponent.h"
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

			uint32_t entity = createEntity();

			VERIFY(entities.at(i).type() == ruc::Json::Type::Object);
			const auto& components = entities.at(i);

			// ID is required
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
			}
			if (components.exists("camera")) {
				auto& camera = addComponent<CameraComponent>(entity);
				components.at("camera").getTo(camera);
			}
			if (components.exists("lua-scripts")) {
				VERIFY(components.at("lua-scripts").type() == ruc::Json::Type::Array);
				const auto& scripts = components.at("lua-scripts").asArray();
				for (size_t j = 0; j < scripts.size(); ++j) {
					auto& script = addComponent<LuaScriptComponent>(entity);
					scripts.at(j).getTo(script);
				}
			}
			if (components.exists("native-scripts")) {
				VERIFY(components.at("native-scripts").type() == ruc::Json::Type::Array);
				const auto& scripts = components.at("native-scripts").asArray();
				for (size_t j = 0; j < scripts.size(); ++j) {
					auto& script = addComponent<NativeScriptComponent>(entity);
					scripts.at(j).getTo(script);
					script.bind();
				}
			}
			if (components.exists("sprite")) {
				auto& sprite = addComponent<SpriteComponent>(entity);
				components.at("sprite").getTo(sprite);
			}
			if (components.exists("text")) {
				auto& text = addComponent<TextAreaComponent>(entity);
				components.at("text").getTo(text);
			}
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
	RenderSystem::the().render();
	TextAreaSystem::the().render();
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
	return createEntityWithUID(UID(), name);
}

uint32_t Scene::createEntityWithUID(UID id, const std::string& name)
{
	uint32_t entity = static_cast<uint32_t>(m_registry->create());
	addComponent<IDComponent>(entity, id);
	addComponent<TagComponent>(entity, name.empty() ? "Unnamed Entity" : name);
	addComponent<TransformComponent>(entity);

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

glm::mat4 Scene::cameraProjectionView()
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
