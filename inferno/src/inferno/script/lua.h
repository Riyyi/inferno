#ifndef LUA_H
#define LUA_H

#include <cstdint> // int32_t, uint32_t
#include <string>  // std::string

#include <lua/lua.h> // lua_State

namespace Inferno {

	struct TransformComponent;

	class Scene;

	class Lua {
	public:
		void initialize();
		void destroy();
		void update(float deltaTime);

		void valid(int32_t error);
		void loadScript();
		void getFunction(const char* table, const char* function);
		void callFunction(int32_t parameters = 0);

	private:
		static void isTable(lua_State* state, int32_t table);
		static void isTable(lua_State* state, const char* table);

		static int32_t getComponent(lua_State* state);
		static int32_t componentIndex(lua_State* state);
		static int32_t componentNewIndex(lua_State* state);

		std::string m_path = "";
		lua_State* m_state = nullptr;

		Scene* m_scene = nullptr;
		uint32_t m_entity = 0;
		TransformComponent* transform = nullptr;

		friend class ScriptSystem;
	};

}

#endif // LUA_H
