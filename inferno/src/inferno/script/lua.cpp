extern "C" {
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
}

#include "inferno/assertions.h"
#include "inferno/file.h"
#include "inferno/scene/components.h"
#include "inferno/scene/scene.h"
#include "inferno/script/lua.h"

namespace Inferno {

	void Lua::initialize()
	{
		m_state = luaL_newstate();

		// Add GetComponent()
		// ---------------------------------

		constexpr int32_t componentUpValueAmount = 1;
		lua_pushlightuserdata(m_state, this);
		lua_pushcclosure(m_state, Lua::getComponent, componentUpValueAmount);
		lua_setglobal(m_state, "GetComponent");

		// Add Metatable
		// ---------------------------------

		luaL_newmetatable(m_state, "ComponentMetatable");

		constexpr int32_t indexUpValueAmount = 1;
		lua_pushstring(m_state, "__index");
		lua_pushlightuserdata(m_state, this);
		lua_pushcclosure(m_state, Lua::componentIndex, indexUpValueAmount);
		lua_settable(m_state, -3); // Pops the string and table

		constexpr int32_t newIndexUpValueAmount = 1;
		lua_pushstring(m_state, "__newindex");
		lua_pushlightuserdata(m_state, this);
		lua_pushcclosure(m_state, Lua::componentNewIndex, newIndexUpValueAmount);
		lua_settable(m_state, -3); // Pops the string and table

		// Load and Initialize Script
		// ---------------------------------

		loadScript();

		getFunction("LuaScript", "initialize");
		callFunction();
	}

	void Lua::destroy()
	{
		getFunction("LuaScript", "destroy");
		callFunction();

		lua_close(m_state);
		m_state = nullptr;
	}

	void Lua::update(float deltaTime)
	{
		getFunction("LuaScript", "update");
		lua_pushnumber(m_state, deltaTime); // Push float argument
		callFunction(1);
	}

	void Lua::valid(int32_t error)
	{
		ASSERT(error == LUA_OK, "{}", lua_tostring(m_state, -1));
	}

	void Lua::loadScript()
	{
		std::string script = File::read(m_path);
		int32_t error = luaL_dostring(m_state, script.c_str());
		valid(error);
	}

	void Lua::getFunction(const char* table, const char* function)
	{
		Lua::isTable(m_state, table);           // Push table
		int32_t tableIdx = lua_gettop(m_state); // Get table stack index
		lua_pushstring(m_state, function);      // Push string
		lua_gettable(m_state, -2);              // Pop string key, push value
		lua_pushvalue(m_state, tableIdx);       // Push table
	}

	void Lua::callFunction(int32_t parameters)
	{
		int32_t error = lua_pcall(m_state, parameters + 1, 0, 0); // Call function()
		valid(error);
	}

	void Lua::isTable(lua_State* state, int32_t table)
	{
		ASSERT(lua_istable(state, table));
	}

	void Lua::isTable(lua_State* state, const char* table)
	{
		lua_getglobal(state, table); // Push table on the stack
		Lua::isTable(state, -1);
	}

	int32_t Lua::getComponent(lua_State* state) // Local scope state, so stack begins at 1!
	{
		ASSERT(lua_gettop(state) == 1, "Lua called with wrong amount of arguments");

		ASSERT(lua_islightuserdata(state, lua_upvalueindex(1)), "Lua did not receive required lightuserdata");
		Lua* self = static_cast<Lua*>(lua_touserdata(state, lua_upvalueindex(1)));

		// Verify parameter
		std::string componentName = lua_tostring(state, -1);
		ASSERT(lua_isstring(state, -1), "Lua did not receive correct parameter");

		// Create userdata
		/*void* pointer =*/ lua_newuserdatauv(state, sizeof(0), 2); // Push userdata
		ASSERT(lua_isuserdata(state, -1), "Lua could not create userdata");

		// Get component
		void* component = nullptr;
		if (componentName.compare("CameraComponent") == 0) {
			component = static_cast<void*>(&self->m_scene->getComponent<CameraComponent>(self->m_entity));
		}

		// Add metatable to userdata
		luaL_getmetatable(state, "ComponentMetatable"); // Push table
		ASSERT(lua_istable(state, -1), "Lua could not find metatable");
		lua_setmetatable(state, -2); // Pop table, set it as metatable onto userdata

		// Add uservalues to userdata
		lua_pushstring(state, componentName.c_str()); // Push string
		lua_setiuservalue(state, -2, 1);              // Pop value, set it as the n-th uservalue onto userdata
		lua_pushlightuserdata(state, component);      // Push pointer
		lua_setiuservalue(state, -2, 2);              // Pop value, set it as the n-th uservalue onto userdata

		return 1; // Return amount of stack items pushed
	}

	int32_t Lua::componentIndex(lua_State* state) // Local scope state, so stack begins at 1!
	{
		ASSERT(lua_gettop(state) == 2, "Lua called with wrong amount of arguments");
		ASSERT(lua_isuserdata(state, -2)); // 1 // <component>.x
		ASSERT(lua_isstring(state, -1));   // 2 // component.<x>

		// ASSERT(lua_islightuserdata(state, lua_upvalueindex(1)), "Lua did not receive required lightuserdata");
		// Lua* self = static_cast<Lua*>(lua_touserdata(state, lua_upvalueindex(1)));

		lua_getiuservalue(state, 1, 1); // Push the n-th uservalue from userdata
		ASSERT(lua_isstring(state, -1), "Lua did not receive component type correctly");
		std::string componentName = lua_tostring(state, -1);

		if (componentName.compare("CameraComponent") == 0) {

			lua_getiuservalue(state, 1, 2); // Push the n-th uservalue from userdata
			ASSERT(lua_islightuserdata(state, -1), "Lua did not receive component pointer correctly");
			CameraComponent* component = (CameraComponent*)lua_touserdata(state, -1);

			std::string index = lua_tostring(state, 2);
			if (index.compare("fov") == 0) {
				lua_pushnumber(state, component->fov);
				return 1;
			}
			else {
				ASSERT_NOT_REACHED();
			}

		}

		return 0;
	}

	int32_t Lua::componentNewIndex(lua_State* state) // Local scope state, so stack begins at 1!
	{
		ASSERT(lua_gettop(state) == 3, "Lua called with wrong amount of arguments");
		ASSERT(lua_isuserdata(state, -3)); // 1 // <component>.x = thing
		ASSERT(lua_isstring(state, -2));   // 2 // component.<x> = thing
		// Value we want to set -1         // 3 // component.x   = <thing>

		lua_getiuservalue(state, 1, 1); // Push the n-th uservalue from userdata
		ASSERT(lua_isstring(state, -1), "Lua did not receive component type correctly");
		std::string componentName = lua_tostring(state, -1);

		if (componentName.compare("CameraComponent") == 0) {

			lua_getiuservalue(state, 1, 2); // Push the n-th uservalue from userdata
			ASSERT(lua_islightuserdata(state, -1), "Lua did not receive component pointer correctly");
			CameraComponent* component = (CameraComponent*)lua_touserdata(state, -1);

			std::string index = lua_tostring(state, 2);
			if (index.compare("fov") == 0) {
				component->fov = lua_tonumber(state, 3);
			}
			else {
				ASSERT_NOT_REACHED();
			}

		}

		return 0;
	}

}
