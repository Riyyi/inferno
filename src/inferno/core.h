#ifndef CORE_H
#define CORE_H

#include <functional> // std::bind, std::placeholders

#define BIT(x) (1 << x)

// @Todo figure out lambda's and replace std::bind
// https://github.com/TheCherno/Hazel/pull/277
//   variadic args in lambdas were added in C++17
#define NF_BIND_EVENT(f) std::bind(&f, this, std::placeholders::_1)

// Compiler
#if defined(__clang__)
	#define GCC
#elif defined(__INTEL_COMPILER) // Supports some GCC extensions
	#define GCC
#elif defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
	#define GCC
#elif defined(_MSC_VER)
	#define MSVC
#endif

#endif // CORE_H
