#pragma once

#include <csignal> // raise
#include <cstdint> // uint32_t
#include <cstdio>  // fflush
#include <cstring> // strlen
#include <fstream> // ifstream
#include <string>

#include "inferno/core.h"
#include "inferno/io/log.h"

#ifndef NDEBUG
	#define NF_ENABLE_ASSERTS
#endif

// Asserts
#ifdef NF_ENABLE_ASSERTS
	// Check if SIGTRAP is available
	#ifdef SIGTRAP
		#define ABORT_SIGNAL SIGTRAP // POSIX
	#else
		#define ABORT_SIGNAL SIGABRT // C99
	#endif

	// Non-standard function macro
	#ifdef GCC
		#define FUNCTION_MACRO __PRETTY_FUNCTION__ // GCC extension
	#elif MSVC
		#define FUNCTION_MACRO __FUNCSIG__
	#else
		#define FUNCTION_MACRO __func__ // C99
	#endif

	// ##__VA_ARGS__ is a non-standard GCC extension, C++20 introduces __VA_OPT__
	// https://stackoverflow.com/questions/52891546/what-does-va-args-mean
	#define ASSERT(expr, ...) (static_cast<bool>(expr) ? (void)0 : Inferno::__assert_fail(#expr, __FILE__, __LINE__, FUNCTION_MACRO, ##__VA_ARGS__))
	#define ASSERT_NOT_REACHED() ASSERT(false)
#else
	#define ASSERT(expr, ...) (static_cast<void>(0))
	#define ASSERT_NOT_REACHED() CRASH()
#endif

#define CRASH() Inferno::__crash();
// https://stackoverflow.com/questions/1440570/likely-unlikely-equivalent-for-msvc

namespace Inferno {

inline void __crash()
{
	asm volatile("int $0x03");
}

	// FIXME: Doesnt print to stderr
	#ifdef NF_ENABLE_ASSERTS
		template<typename... P>
		inline void __assert_fail(const char* assertion, const char* file, uint32_t line, const char* function, P&&... parameters)
		{
			(void)function;

			// Get the line that caused the error
			std::ifstream source(file);
			std::string content;
			if (source.is_open()) {
				for (uint32_t i = 0; std::getline(source, content); ++i) {
					if (i == line - 1) {
						break;
					}
				}
			}
			// Replace tab indentation with spaces
			size_t tabs = content.find_first_not_of('\t');
			if (tabs > 0 && tabs < content.size()) {
				content = std::string(tabs * 4, ' ') + content.substr(tabs);
			}

			// Find the assertion in the line
			size_t column = content.find_first_of(assertion);
			size_t assertionLength = strlen(assertion);
			if (column == std::string::npos) {
				column = content.find_first_not_of(' ');
				assertionLength = content.length() - column;
			}

			// Error message
			dbgln(false, "\033[0;1m{}:{}:{}: ", file, line, column + 1);
			dangerln(false, "error: ");
			dbgln(false, "assertion failed");
			if (sizeof...(P) > 0) {
				dbgln(false, ": ");
				dbgln(Log::None, false, std::forward<P>(parameters)...);
			}

			// Code line
			dbgln("\n {} | {}\033[31;1m{}\033[0m{}", line,
	              content.substr(0, column),                 // Whitespace at front
	              content.substr(column, assertionLength),   // Error portion
	              content.substr(column + assertionLength)); // Rest of the line

			// Arrow pointer
			dbgln(" {} | {}\033[31;1m^{}\033[0m",
	              std::string(std::to_string(line).length(), ' '), // Line number spacing
	              std::string(column, ' '),                        // Content spacing
	              std::string(assertionLength - 1, '~'));          // Arrow pointer

			fflush(stdout); // FIXME: stdout is buffered, strerr is not so wouldnt need this
			CRASH();
		}
	#endif

}

// https://github.com/scottt/debugbreak
