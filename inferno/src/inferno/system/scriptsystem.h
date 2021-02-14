#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include <cstdint> // uint32_t

#include "inferno/singleton.h"

namespace Inferno {

	struct NativeScriptComponent;
	struct LuaScriptComponent;

	class Scene;

	class ScriptSystem final : public Singleton<ScriptSystem> {
	public:
		ScriptSystem(s);
		virtual ~ScriptSystem();

		void update(float deltaTime);

		void cleanup(uint32_t entity);
		void cleanup(NativeScriptComponent& nativeScript);
		void cleanup(LuaScriptComponent& luaScript);

		void setScene(Scene* scene) { m_scene = scene; }

	private:
		Scene* m_scene;
	};

}

#endif // SCRIPT_SYSTEM_H
