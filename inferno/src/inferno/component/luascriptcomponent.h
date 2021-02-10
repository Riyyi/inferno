#ifndef LUA_SCRIPT_COMPONENT_H
#define LUA_SCRIPT_COMPONENT_H

#include <string>  // std::string
#include <utility> // std::move

namespace Inferno {

	class LuaScript;

	struct LuaScriptComponent {
		LuaScript* instance = nullptr;
		std::string path;

		// Dont allow manually setting instance during construction
		LuaScriptComponent() {}
		LuaScriptComponent(const std::string& path)
			: path(std::move(path)) {}
	};
}

#endif // LUA_SCRIPT_COMPONENT_H
