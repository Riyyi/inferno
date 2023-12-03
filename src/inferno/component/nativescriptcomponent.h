/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include "inferno/script/nativescript.h"

namespace Inferno {

struct NativeScriptComponent {
	NativeScript* instance { nullptr };

	NativeScript::InitializeFunction initialize { nullptr };
	NativeScript::DestroyFunction destroy { nullptr };

	// Dont allow manually setting instance during construction
	NativeScriptComponent() {}
	NativeScriptComponent(const std::string& binding)
	{
		initialize = NativeScriptBinding::the().initializeBinding(binding);
		destroy = NativeScriptBinding::the().destroyBinding(binding);
	}
};

} // namespace Inferno
