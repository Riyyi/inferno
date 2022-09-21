#pragma once

#include "ruc/meta/assert.h"

#include "inferno/script/nativescript.h"

namespace Inferno {

struct NativeScriptComponent {
	NativeScript* instance { nullptr };

	NativeScript* (*initialize)();

	// Dont allow manually setting instance during construction
	NativeScriptComponent() {}

	template<typename T>
	void bind()
	{
		VERIFY(instance == nullptr, "NativeScript already bound");
		initialize = []() { return static_cast<NativeScript*>(new T()); };
	}

	void destroy()
	{
		VERIFY(instance, "Attempting to destroy an uninitialized NativeScript");
		delete instance;
		instance = nullptr;
	}
};

} // namespace Inferno
