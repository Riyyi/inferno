#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <limits> // std::numeric_limits
#include <string> // std::string, std::stoul

#include "inferno/assert.h"

namespace std {

	// Can't believe this is not in the standard library

	inline uint32_t stou(const std::string& string)
	{
		unsigned long size = std::stoul(string);
		ASSERT(size <= std::numeric_limits<uint32_t>::max(), "String util not in uint32_t range '{}'", string);
		return static_cast<uint32_t>(size);
	}

	inline uint32_t stou(const char* string)
	{
		return stou(std::string(string));
	}

} // namespace std


#endif // UTIL_STRING_H
