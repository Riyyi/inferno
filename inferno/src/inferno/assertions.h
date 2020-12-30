#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include <csignal> // raise

#include "inferno/core.h"
#include "inferno/log.h"

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
	#endif

	// ##__VA_ARGS__ is a non-standard GCC extension, C++20 introduces __VA_OPT__
	// https://stackoverflow.com/questions/52891546/what-does-va-args-mean
	#define ASSERT(cond, ...) static_cast<bool>(cond) ? (void)0 : __assertion_failed(#cond, __FILE__, __LINE__, FUNCTION_MACRO, ##__VA_ARGS__)
	#define ASSERT_NOT_REACHED() ASSERT(false)
#else
	#define ASSERT(cond, ...)
	#define ASSERT_NOT_REACHED() CRASH()
#endif

#define CRASH() raise(SIGABRT)

namespace Inferno {

	#ifdef NF_ENABLE_ASSERTS
	template<typename... P>
	void __assertion_failed(const char* message, const char* file, unsigned line, const char* function, const P&... parameters)
	{
		dbg(Log::Danger, false) << "ASSERTION FAILED: " << message;

		if (sizeof...(P) > 0) {
			dbg(Log::Danger, false) << ": ";
			dbgln(Log::Danger, false, parameters...);
			dbg(Log::Danger, false);
		}

		dbg(Log::Danger) << "\n\t" << file << ":" << line << ": " << function;

		raise(ABORT_SIGNAL);
	}
	#endif

}

#endif // ASSERTIONS_H
