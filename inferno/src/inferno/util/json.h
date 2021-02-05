#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <optional> // std::optional

#include "nlohmann/json.hpp"

#include "inferno/assert.h"

namespace Inferno {

	using json = nlohmann::json;
	using json_const_iterator = nlohmann::json::const_iterator;

	class Json {
	public:
		static const json& getValue(json_const_iterator& it)
		{
			return it.value();
		}

		template<typename T, typename C>
		static std::optional<T> getType(const json& json, C check)
		{
			if (json.type() == check) {
				return json.get<T>();
			}

			return {};
		}

		// ---------------------------------

		static bool hasProperty(const json& json, json_const_iterator& it, const char* member)
		{
			it = json.find(member);
			return it != json.end();
		}

		// ---------------------------------

		template<typename T, typename C>
		static std::optional<T> parseProperty(const json& json, const char* property, bool required, C check)
		{
			bool result;
			json_const_iterator it;

			// Has property
			result = hasProperty(json, it, property);
			ASSERT(!required || (required && result), "Json could not find property '{}'", property);

			if (!result) {
				return {};
			}

			// Return data found in iterator, empty if incorrect type
			return getType<T>(getValue(it), check);
		}
	};

} // namespace Inferno


#endif // JSON_UTIL_H
