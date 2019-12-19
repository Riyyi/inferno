#ifndef CORE_H
#define CORE_H

#include <csignal>    // raise
#include <cstdio>     // sprintf
#include <cstring>    // strlen
#include <functional> // std::bind

#define BIT(x) (1 << x)

#define NF_BIND_EVENT(f) std::bind(&f, this, std::placeholders::_1)

// Debugging defines
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

	#define NF_ASSERT(x, y, ...) if(!(x)) { \
		char buffer[15 + strlen(y)]; \
		sprintf(buffer, "Assert: {%%s}, %s", y); \
		NF_DANGER(buffer, #x, ##__VA_ARGS__); raise(ABORT_SIGNAL); }
	#define NF_CORE_ASSERT(x, y, ...) if(!(x)) { \
		char buffer[15 + strlen(y)]; \
		sprintf(buffer, "Assert: {%%s}, %s", y); \
		NF_CORE_DANGER(buffer, #x, ##__VA_ARGS__); raise(ABORT_SIGNAL); }
#else
	#define NF_ASSERT(x, y)
	#define NF_CORE_ASSERT(x, y)
#endif

#endif // CORE_H
