#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::radians

#include "inferno/component/transformcomponent.h"
#include "inferno/io/log.h"
#include "inferno/system/transformsystem.h"

namespace Inferno {

	TransformSystem::TransformSystem(s)
	{
		info() << "TransformSystem initialized";
	}

	TransformSystem::~TransformSystem()
	{
	}

	void TransformSystem::update()
	{
		auto view = m_registry->view<TransformComponent>();

		for (auto entity : view) {

			auto& component = view.get<TransformComponent>(entity);

			// Identity matrix
			component.transform = glm::mat4(1.0f);

			// Translate
			component.transform = glm::translate(component.transform, component.translate);

			// Rotate
			component.transform = glm::rotate(component.transform, glm::radians(component.rotate.x), {1.0, 0.0, 0.0});
			component.transform = glm::rotate(component.transform, glm::radians(component.rotate.y), {0.0, 1.0, 0.0});
			component.transform = glm::rotate(component.transform, glm::radians(component.rotate.z), {0.0, 0.0, 1.0});

			// Scale
			component.transform = glm::scale(component.transform, component.scale);
		}
	}

} // namespace Inferno
