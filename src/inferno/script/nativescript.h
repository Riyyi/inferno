#pragma once

#include "inferno/scene/scene.h"

namespace Inferno {

	struct TransformComponent;

	class NativeScript {
	public:
		virtual ~NativeScript() {}

	protected:
		virtual void initialize() {}
		virtual void destroy() {}
		virtual void update(float deltaTime) { (void)deltaTime; }

		template<typename T>
		T& getComponent() const
		{
			return m_scene->getComponent<T>(m_entity);
		}

		TransformComponent* transform { nullptr };

	private:
		Scene* m_scene { nullptr };
		uint32_t m_entity { 0 };

		friend class ScriptSystem;
	};

} // namespace Inferno
