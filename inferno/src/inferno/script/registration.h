#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "sol/overload.hpp"   // sol::overload
#include "sol/state_view.hpp" // sol::state_view

namespace Inferno {

	class Registration final {
	public:
		static void fill(sol::state_view& state);

	private:
		static void glm(sol::state_view& state);
		static void component(sol::state_view& state);
		static void input(sol::state_view& state);

		template<typename T, typename V>
		static auto addition()
		{
			return sol::overload(
				[](const T& lhs, const T& rhs) { return lhs + rhs; },
				[](const T& lhs, const V& rhs) { return lhs + rhs; },
				[](const V& lhs, const T& rhs) { return lhs + rhs; }
			);
		}

		template<typename T, typename V>
		static auto subtraction()
		{
			return sol::overload(
				[](const T& lhs, const T& rhs) { return lhs - rhs; },
				[](const T& lhs, const V& rhs) { return lhs - rhs; },
				[](const V& lhs, const T& rhs) { return lhs - rhs; }
			);
		}

		template<typename T, typename V>
		static auto multiplication()
		{
			return sol::overload(
				[](const T& lhs, const T& rhs) { return lhs * rhs; },
				[](const T& lhs, const V& rhs) { return lhs * rhs; },
				[](const V& lhs, const T& rhs) { return lhs * rhs; }
			);
		}

		template<typename T, typename V>
		static auto division()
		{
			return sol::overload(
				[](const T& lhs, const T& rhs) { return lhs / rhs; },
				[](const T& lhs, const V& rhs) { return lhs / rhs; },
				[](const V& lhs, const T& rhs) { return lhs / rhs; }
			);
		}
	};

}

#endif // REGISTRATION_H
