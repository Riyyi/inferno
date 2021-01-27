#ifndef LUA_H
#define LUA_H

#include <cstdint> // uint32_t
#include <string>  // std::string

#define SOL_ALL_SAFETIES_ON 1
#include "sol/protected_function.hpp"
#include "sol/protected_function_result.hpp"
#include "sol/state.hpp"
#include "sol/table.hpp"

#include "inferno/assertions.h"

namespace Inferno {

	struct TransformComponent;

	class Scene;

	class Lua {
	public:
		void initialize();
		void destroy();
		void update(float deltaTime);

		void loadScript();

	private:
		sol::table getTable(const char* name);

		template<typename... P>
		void callFunction(const char* table, const char* function, P&&... parameters)
		{
			sol::table solTable = getTable(table);
			sol::protected_function solFunction = solTable[function];

			// Only call function if it exists
			if (solFunction.valid()) {
				sol::protected_function_result result = solFunction(solTable, parameters...);
				ASSERT(result.valid(), "Lua function {}", ((sol::error)result).what());
			}
		}

		sol::state m_state;
		std::string m_path = "";

		Scene* m_scene = nullptr;
		uint32_t m_entity = 0;
		TransformComponent* transform = nullptr;

		friend class ScriptSystem;
	};

}

#endif // LUA_H
