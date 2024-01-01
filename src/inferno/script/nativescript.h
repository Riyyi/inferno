/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "ruc/meta/core.h"
#include "ruc/singleton.h"

#include "inferno/scene/scene.h"

namespace Inferno {

struct TransformComponent;

class NativeScript {
public:
	typedef NativeScript* (*InitializeFunction)();
	typedef void (*DestroyFunction)(NativeScript*);

	virtual ~NativeScript() {}

protected:
	virtual void initialize() {}
	virtual void destroy() {}
	virtual void update(float deltaTime) { (void)deltaTime; }

	template<typename T>
	T& getComponent() const
	{
		return m_scene->getComponent<T>(m_entity);
	}

	TransformComponent* transform { nullptr };

private:
	Scene* m_scene { nullptr };
	uint32_t m_entity { 0 };

	friend class ScriptSystem;
};

class NativeScriptBinding final : public ruc::Singleton<NativeScriptBinding> {
public:
	NativeScriptBinding(s) {}
	virtual ~NativeScriptBinding() {}

	void registerBinding(const std::string& binding,
	                     NativeScript::InitializeFunction initialize,
	                     NativeScript::DestroyFunction destroy)
	{
		m_initializeBindings.emplace(binding, initialize);
		m_detroyBindings.emplace(binding, destroy);
	}

	NativeScript::InitializeFunction initializeBinding(const std::string& name) { return m_initializeBindings[name]; }
	NativeScript::DestroyFunction destroyBinding(const std::string& name) { return m_detroyBindings[name]; }

private:
	std::unordered_map<std::string, NativeScript::InitializeFunction> m_initializeBindings;
	std::unordered_map<std::string, NativeScript::DestroyFunction> m_detroyBindings;
};

} // namespace Inferno

// TODO: Move this to ruc
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define CONCAT_IMPL(a, b) a##b

#define BIND_NATIVE_IMPL(name, struct_name)                                                                 \
	struct struct_name {                                                                                    \
		struct_name()                                                                                       \
		{                                                                                                   \
			Inferno::NativeScriptBinding::the().registerBinding(                                            \
				#name,                                                                                      \
				[]() -> Inferno::NativeScript* { return static_cast<Inferno::NativeScript*>(new name()); }, \
				[](Inferno::NativeScript* nativeScript) -> void {                                           \
					VERIFY(nativeScript, "Attempting to destroy an uninitialized NativeScript");            \
					delete static_cast<name*>(nativeScript);                                                \
					nativeScript = nullptr;                                                                 \
				});                                                                                         \
		}                                                                                                   \
	};                                                                                                      \
	static struct struct_name struct_name; // NOLINT(clang-diagnostic-unused-function)

#define BIND_NATIVE(name) \
	BIND_NATIVE_IMPL(name, CONCAT(__BIND_NATIVE_FUNCTION_, __COUNTER__))
