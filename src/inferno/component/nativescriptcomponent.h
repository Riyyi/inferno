/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <utility> // std::move

#include "inferno/script/nativescript.h"

namespace Inferno {

struct NativeScriptComponent {
	NativeScript* instance { nullptr };
	std::string name;

	NativeScript::InitializeFunction initialize { nullptr };
	NativeScript::DestroyFunction destroy { nullptr };

	// Dont allow manually setting instance during construction
	NativeScriptComponent() {}
	NativeScriptComponent(const std::string& name)
		: name(std::move(name))
	{
		bind();
	}

	void bind()
	{
		VERIFY(initialize == nullptr && destroy == nullptr, "NativeScript already bound");

		VERIFY(name != "", "name not set");
		initialize = NativeScriptBinding::the().initializeBinding(name);
		destroy = NativeScriptBinding::the().destroyBinding(name);
	}
};

void fromJson(const ruc::Json& json, NativeScriptComponent& value);

} // namespace Inferno
