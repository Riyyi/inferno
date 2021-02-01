#ifndef ASSERT_H
#define ASSERT_H

#include <csignal> // raise

#include "inferno/core.h"
#include "inferno/io/log.h"

#ifndef NDEBUG
	#define NF_ENABLE_ASSERTS
#endif

// Asserts
#ifdef NF_ENABLE_ASSERTS
	// Check if SIGTRAP is available
	#ifdef SIGTRAP
		#define ABORT_SIGNAL SIGTRAP
	#else
		#define ABORT_SIGNAL SIGABRT
	#endif

	// Non-standard function macro
	#ifdef GCC
		#define FUNCTION_MACRO __PRETTY_FUNCTION__
	#elif MSVC
		#define FUNCTION_MACRO __FUNCSIG__
	#else
		#define FUNCTION_MACRO __func__
	#endif

	// ##__VA_ARGS__ is a non-standard GCC extension, C++20 introduces __VA_OPT__
	// https://stackoverflow.com/questions/52891546/what-does-va-args-mean
	#define ASSERT(expr, ...) static_cast<bool>(expr) ? (void)0 : Inferno::__assert_fail(#expr, __FILE__, __LINE__, FUNCTION_MACRO, ##__VA_ARGS__)
	#define ASSERT_NOT_REACHED() ASSERT(false)
#else
	#define ASSERT(expr, ...)
	#define ASSERT_NOT_REACHED() CRASH()
#endif

#define CRASH() raise(SIGABRT)

namespace Inferno {

	#ifdef NF_ENABLE_ASSERTS
		template<typename... P>
		[[noreturn]] inline void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function, P&&... parameters)
		{
			dangerln(false, "ASSERTION `{}' FAILED.", assertion);

			if (sizeof...(P) > 0) {
				dbg(false) << " ";
				dbgln(Log::Danger, false, std::forward<P>(parameters)...);
			}

			danger() << "\n\t" << file << ":" << line << ": " << function;

			raise(ABORT_SIGNAL);
		}
	#endif

}

#endif // ASSERT_H
