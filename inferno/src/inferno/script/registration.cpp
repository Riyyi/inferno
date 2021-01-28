#include "glm/ext/vector_float2.hpp"    // glm::vec2
#include "glm/ext/vector_float3.hpp"    // glm::vec3
#include "glm/ext/vector_float4.hpp"    // glm::vec4
#include "glm/ext/matrix_transform.hpp" // glm::radians

#include "inferno/input.h"
#include "inferno/inputcodes.h"
#include "inferno/scene/components.h"
#include "inferno/script/registration.h"

namespace Inferno {

	void Registration::fill(sol::state_view &state)
	{
		glm(state);
		component(state);
		input(state);
	}

	void Registration::glm(sol::state_view& state)
	{
		auto glm = state["glm"].get_or_create<sol::table>();

		auto vec2 = glm.new_usertype<glm::vec2>(
			"vec2",
			sol::call_constructor, sol::constructors<glm::vec2(), glm::vec2(glm::vec2), glm::vec2(float), glm::vec2(float, float)>(),
			"x", &glm::vec2::x, "y", &glm::vec2::y,
			"r", &glm::vec2::r, "g", &glm::vec2::g,
			"s", &glm::vec2::s, "t", &glm::vec2::t,
			"__add", addition<glm::vec2, float>(),
			"__sub", subtraction<glm::vec2, float>(),
			"__mul", multiplication<glm::vec2, float>(),
			"__div", division<glm::vec2, float>()
		);

		auto vec3 = glm.new_usertype<glm::vec3>(
			"vec3",
			sol::call_constructor, sol::constructors<glm::vec3(), glm::vec3(glm::vec3), glm::vec3(float), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z,
			"r", &glm::vec3::r, "g", &glm::vec3::g, "b", &glm::vec3::b,
			"s", &glm::vec3::s, "t", &glm::vec3::t, "p", &glm::vec3::p,
			"__add", addition<glm::vec3, float>(),
			"__sub", subtraction<glm::vec3, float>(),
			"__mul", multiplication<glm::vec3, float>(),
			"__div", division<glm::vec3, float>()
			// "__tostring", [](glm::vec3 self) { return self.x; } // Template!!! convert via logstream
		);

		auto vec4 = glm.new_usertype<glm::vec4>(
			"vec4",
			sol::call_constructor, sol::constructors<glm::vec4(), glm::vec4(glm::vec4), glm::vec4(float), glm::vec4(float, float, float, float)>(),
			"x", &glm::vec4::x, "y", &glm::vec4::y, "z", &glm::vec4::z, "w", &glm::vec4::w,
			"r", &glm::vec4::r, "g", &glm::vec4::g, "b", &glm::vec4::b, "a", &glm::vec4::a,
			"s", &glm::vec4::s, "t", &glm::vec4::t, "p", &glm::vec4::p, "q", &glm::vec4::q,
			"__add", addition<glm::vec4, float>(),
			"__sub", subtraction<glm::vec4, float>(),
			"__mul", multiplication<glm::vec4, float>(),
			"__div", division<glm::vec4, float>()
		);

		glm.set_function("radians", sol::overload(
			 [](float v) { return glm::radians(v); },
			 [](const glm::vec2& v) { return glm::radians(v); },
			 [](const glm::vec3& v) { return glm::radians(v); },
			 [](const glm::vec4& v) { return glm::radians(v); }
		));

		glm.set_function("normalize", sol::overload(
			 [](const glm::vec2& v) { return glm::normalize(v); },
			 [](const glm::vec3& v) { return glm::normalize(v); },
			 [](const glm::vec4& v) { return glm::normalize(v); }
		));

		glm.set_function("cross", [](const glm::vec3& x, const glm::vec3& y) { return glm::cross(x, y); });
	}

	void Registration::component(sol::state_view& state)
	{
		auto tagComponent = state.new_usertype<TagComponent>("TagComponent", sol::no_constructor);
		tagComponent["tag"] = &TagComponent::tag;

		auto transformComponent = state.new_usertype<TransformComponent>("TransformComponent", sol::no_constructor);
		transformComponent["translate"] = &TransformComponent::translate;
		transformComponent["rotate"]    = &TransformComponent::rotate;
		transformComponent["scale"]     = &TransformComponent::scale;
		transformComponent["transform"] = &TransformComponent::transform;

		auto cameraType = state.new_enum("CameraType",
		                                 "Orthographic", CameraType::Orthographic,
		                                 "Perspective", CameraType::Perspective);

		auto cameraComponent = state.new_usertype<CameraComponent>("CameraComponent", sol::no_constructor);
		cameraComponent["type"]       = &CameraComponent::type;
		cameraComponent["zoomLevel"]  = &CameraComponent::zoomLevel;
		cameraComponent["rotateAxis"] = &CameraComponent::rotateAxis;
		cameraComponent["fov"]        = &CameraComponent::fov;
		cameraComponent["pitch"]      = &CameraComponent::pitch;
		cameraComponent["yaw"]        = &CameraComponent::yaw;
		cameraComponent["up"]         = &CameraComponent::up;
		cameraComponent["projection"] = &CameraComponent::projection;

		auto spriteComponent = state.new_usertype<SpriteComponent>("SpriteComponent", sol::no_constructor);
		spriteComponent["color"]   = &SpriteComponent::color;
		spriteComponent["texture"] = &SpriteComponent::texture;
	}

	void Registration::input(sol::state_view& state)
	{
		state.set_function("keyCode", &KeyCode);
		// state["keyCode"] = &KeyCode;

		auto input = state.new_usertype<Input>("Input", sol::no_constructor);
		input["isKeyPressed"] = &Input::isKeyPressed;
		input["getXOffset"] = &Input::getXOffset;
		input["getYOffset"] = &Input::getYOffset;
	}
}
