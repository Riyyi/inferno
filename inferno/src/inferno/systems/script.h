#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include <cstdint> // uint32_t

namespace Inferno {

	struct NativeScriptComponent;
	struct LuaScriptComponent;

	class Scene;

	class ScriptSystem {
	public:
		void initialize();
		void destroy();
		void update(float deltaTime);

		void cleanup(uint32_t entity);
		void cleanup(NativeScriptComponent& nativeScript);
		void cleanup(LuaScriptComponent& luaScript);

		void setScene(Scene* scene) { m_scene = scene; }

		static inline ScriptSystem& the() { return *s_instance; }

	private:
		Scene* m_scene;

		static ScriptSystem* s_instance;
	};

}

#endif // SCRIPT_SYSTEM_H
