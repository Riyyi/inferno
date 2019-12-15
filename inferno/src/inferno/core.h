#ifndef CORE_H
#define CORE_H

#include <csignal>

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

	#define NF_ASSERT(x, y)      if(!(x)) { NF_DANGER("Assert: {%s}, %s", #x, y); raise(ABORT_SIGNAL); }
	#define NF_CORE_ASSERT(x, y) if(!(x)) { NF_CORE_DANGER("Assert: {%s}, %s", #x, y); raise(ABORT_SIGNAL); }
#else
	#define NF_ASSERT(x, y)
	#define NF_CORE_ASSERT(x, y)
#endif

#endif // CORE_H
