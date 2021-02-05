#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <cstdint>  // int32_t, uint32_t
#include <optional> // std::optional
#include <vector>   // std::vector

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

		template<typename T>
		static std::optional<T> getPropertyValue(const json& json, json::value_t check)
		{
			if (json.type() == check) {
				return json.get<T>();
			}

			return {};
		}

		template<typename T>
		static std::optional<T> getPropertyValue(const json& json, std::vector<json::value_t> checks)
		{
			bool valid = false;

			// Check values with all allowed types
			for (auto check : checks) {
				if (json.type() == check) {
					valid = true;
					break;
				}
			}

			// Get value if valid type
			if (valid) {
				return json.get<T>();
			}

			return {};
		}

		// ---------------------------------

		static bool hasProperty(const json& json, const char* property)
		{
			return json.find(property) != json.end();
		}

		static bool hasProperty(const json& json, json_const_iterator& it, const char* property)
		{
			it = json.find(property);
			return it != json.end();
		}

		// ---------------------------------

		template<typename T>
		static std::optional<T> parseProperty(const json& json, const char* property, bool required, json::value_t check)
		{
			return parseProperty<T>(json, property, required, std::vector { check });
		}

		template<typename T>
		static std::optional<T> parseProperty(const json& json, const char* property, bool required, std::vector<json::value_t> checks)
		{
			bool exists;
			json_const_iterator it;

			// Has property
			exists = hasProperty(json, it, property);
			ASSERT(!required || (required && exists), "Json could not find required property '{}'", property);

			if (!exists) {
				return {};
			}

			// Return data found in iterator, empty if incorrect type
			return getPropertyValue<T>(getValue(it), checks);
		}

		template<typename T>
		static std::optional<std::vector<T>> parseArrayProperty(const json& json, const char* property, bool required, json::value_t check)
		{
			return parseArrayProperty<T>(json, property, required, std::vector { check });
		}

		template<typename T>
		static std::optional<std::vector<T>> parseArrayProperty(const json& json, const char* property, bool required, std::vector<json::value_t> checks)
		{
			bool exists;
			json_const_iterator it;

			// Has property
			exists = hasProperty(json, it, property);
			ASSERT(!required || (required && exists), "Json could not find required property '{}'", property);

			if (!exists) {
				return {};
			}

			// Check if property is array
			ASSERT(getValue(it).is_array(), "Json property is not an array '{}'", property);

			// Fill array with values
			std::vector<T> values;
			for (auto value : getValue(it)) {
				auto typedValue = getPropertyValue<T>(value, checks);
				if (typedValue) {
					values.emplace_back(typedValue.value());
				}
				else {
					warnln("Json array property '{}' has type inconsistency", property);
				}
			}

			return values.size() > 0 ? std::optional { values } : std::nullopt;
		}

		static std::optional<bool> parseBoolProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<bool>(json, property, required, json::value_t::boolean);
		}

		static std::optional<double> parseDoubleProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<double>(json, property, required, std::vector { json::value_t::number_integer, json::value_t::number_unsigned, json::value_t::number_float });
		}

		static std::optional<int32_t> parseIntegerProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<int32_t>(json, property, required, json::value_t::number_integer);
		}

		static std::optional<uint32_t> parseUnsignedProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<uint32_t>(json, property, required, json::value_t::number_unsigned);
		}

		static std::optional<std::string> parseStringProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<std::string>(json, property, required, json::value_t::string);
		}

		static std::optional<std::vector<double>> parseDoubleArrayProperty(const json& json, const char* property, bool required)
		{
			return parseArrayProperty<double>(json, property, required, std::vector { json::value_t::number_integer, json::value_t::number_unsigned, json::value_t::number_float });
		}

		static std::optional<std::vector<int32_t>> parseIntegerArrayProperty(const json& json, const char* property, bool required)
		{
			return parseArrayProperty<int32_t>(json, property, required, json::value_t::number_integer);
		}

		static std::optional<std::vector<uint32_t>> parseUnsignedArrayProperty(const json& json, const char* property, bool required)
		{
			return parseArrayProperty<uint32_t>(json, property, required, json::value_t::number_unsigned);
		}

	};

} // namespace Inferno

#endif // JSON_UTIL_H
