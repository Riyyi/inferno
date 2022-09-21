#pragma once

#include "inferno/assert.h"
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
			ASSERT(instance == nullptr, "NativeScript already bound");
			initialize = []() { return static_cast<NativeScript*>(new T()); };
		}

		void destroy() {
			ASSERT(instance, "Attempting to destroy an uninitialized NativeScript");
			delete instance;
			instance = nullptr;
		}
	};

} // namespace Inferno
