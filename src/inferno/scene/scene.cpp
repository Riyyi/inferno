/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/file.h"
#include "ruc/format/log.h"
#include "ruc/json/json.h"
#include "ruc/meta/assert.h"

#include "inferno/component/cameracomponent.h"
#include "inferno/component/luascriptcomponent.h"
#include "inferno/component/nativescriptcomponent.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/tagcomponent.h"
#include "inferno/component/textareacomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/scene/scene.h"
#include "inferno/script/cameracontroller.h"
#include "inferno/script/nativescript.h"
#include "inferno/system/camerasystem.h"
#include "inferno/system/rendersystem.h"
#include "inferno/system/scriptsystem.h"
#include "inferno/system/textareasystem.h"
#include "inferno/system/transformsystem.h"

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

	// Camera

	VERIFY(sceneJson.exists("camera"), "scene doesnt contain a camera");
	auto& cameraJson = sceneJson.at("camera");
	uint32_t camera = loadEntity(cameraJson);

	auto cameraType = CameraType::Perspective;
	if (cameraJson.exists("type") && cameraJson.at("type").get<std::string>() == "orthographic") {
		cameraType = CameraType::Orthographic;
	}
	addComponent<CameraComponent>(camera, cameraType);

	if (cameraJson.exists("script")) {
		auto& cameraScript = cameraJson.at("script");
		if (cameraScript.exists("type") && cameraScript.exists("name")) {
			if (cameraScript.at("type").get<std::string>() == "lua") {
				addComponent<LuaScriptComponent>(camera, cameraScript.at("name").get<std::string>());
			}
			else {
				addComponent<NativeScriptComponent>(camera).bind<CameraController>();
			}
		}
	}

	// Quads

	if (sceneJson.exists("quad") && sceneJson.at("quad").type() == ruc::Json::Type::Array) {
		auto& quads = sceneJson.at("quad").asArray().elements();
		for (const auto& quad : quads) {
			uint32_t quadEntity = loadEntity(quad);
			addComponent<SpriteComponent>(quadEntity);
			auto& spriteComponent = getComponent<SpriteComponent>(quadEntity);
			quad.getTo(spriteComponent);
		}
	}

	// Text

	uint32_t text = createEntity("Text");
	addComponent<TextAreaComponent>(text, "HelloWorld!", "assets/fnt/dejavu-sans", 0, 150, 3);
	// addComponent<TextAreaComponent>(text, "@#$%^&*()qygij!", "assets/fnt/dejavu-sans-test", 0, 150, 3);

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

uint32_t Scene::loadEntity(ruc::Json json)
{
	uint32_t entity = createEntity((json.exists("name"))
	                                   ? json.at("name").get<std::string>()
	                                   : "");
	auto& transform = getComponent<TransformComponent>(entity);
	json.getTo(transform);

	return entity;
}

glm::mat4 Scene::cameraProjectionView()
{
	return CameraSystem::the().projectionView();
}

void Scene::validEntity(uint32_t entity) const
{
	VERIFY(m_registry->valid(entt::entity { entity }), "Entity is not valid");
}

} // namespace Inferno

void ruc::format::Formatter<entt::entity>::format(Builder& builder, entt::entity value) const
{
	return Formatter<uint32_t>::format(builder, static_cast<uint32_t>(value));
}
