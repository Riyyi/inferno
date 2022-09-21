#pragma once

#include <string>  // std::string
#include <utility> // std::move

namespace Inferno {

	class LuaScript;

	struct LuaScriptComponent {
		LuaScript* instance { nullptr };
		std::string path;

		// Dont allow manually setting instance during construction
		LuaScriptComponent() {}
		LuaScriptComponent(const std::string& path)
			: path(std::move(path)) {}
	};
} // namespace Inferno
