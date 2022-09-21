#pragma once

#if 0
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

			// Check if property is array []
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

			// Return vector if it has any items, uninitialized optional otherwise
			if (values.size() > 0) return values;
			return {};
		}

		template<typename T>
		static std::optional<std::map<std::string, T>> parseObjectProperty(const json& json, const char* property, bool required, json::value_t check)
		{
			return parseObjectProperty<T>(json, property, required, std::vector { check });
		}

		template<typename T>
		static std::optional<std::map<std::string, T>> parseObjectProperty(const json& json, const char* property, bool required, std::vector<json::value_t> checks)
		{
			bool exists;
			json_const_iterator it;

			// Has property
			exists = hasProperty(json, it, property);
			ASSERT(!required || (required && exists), "Json could not find required property '{}'", property);

			if (!exists) {
				return {};
			}

			// Check if property is an object {}
			ASSERT(getValue(it).is_object(), "Json property is not an array '{}'", property);

			std::map<std::string, T> values;
			for (auto& [key, value] : getValue(it).items()) {
				auto typedValue = getPropertyValue<T>(value, checks);
				if (typedValue) {
					values.emplace(std::move(key), typedValue.value());
				}
				else {
					warnln("Json array property '{}' has type inconsistency", property);
				}
			}

			// Return map if it has any items, uninitialized optional otherwise
			if (values.size() > 0) return values;
			return {};
		}

		static std::optional<bool> parseBoolProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<bool>(json, property, required, json::value_t::boolean);
		}

		static std::optional<float> parseFloatProperty(const json& json, const char* property, bool required)
		{
			auto result = parseProperty<double>(json, property, required, std::vector { json::value_t::number_integer, json::value_t::number_unsigned, json::value_t::number_float });
			if (result) return static_cast<float>(result.value());
			return {};
		}

		static std::optional<int32_t> parseIntegerProperty(const json& json, const char* property, bool required)
		{
			auto result = parseProperty<int64_t>(json, property, required, json::value_t::number_integer);
			if (result) return static_cast<int32_t>(result.value());
			return {};
		}

		static std::optional<uint32_t> parseUnsignedProperty(const json& json, const char* property, bool required)
		{
			auto result = parseProperty<uint64_t>(json, property, required, json::value_t::number_unsigned);
			if (result) return static_cast<uint32_t>(result.value());
			return {};
		}

		static std::optional<std::string> parseStringProperty(const json& json, const char* property, bool required)
		{
			return parseProperty<std::string>(json, property, required, json::value_t::string);
		}

		static std::optional<std::vector<float>> parseFloatArrayProperty(const json& json, const char* property, bool required)
		{
			auto result = parseArrayProperty<double>(json, property, required, std::vector { json::value_t::number_integer, json::value_t::number_unsigned, json::value_t::number_float });
			if (result) return std::vector<float>(result.value().begin(), result.value().end());
			return {};
		}

		static std::optional<std::vector<int32_t>> parseIntegerArrayProperty(const json& json, const char* property, bool required)
		{
			auto result = parseArrayProperty<int64_t>(json, property, required, json::value_t::number_integer);
			if (result) return std::vector<int32_t>(result.value().begin(), result.value().end());
			return {};
		}

		static std::optional<std::vector<uint32_t>> parseUnsignedArrayProperty(const json& json, const char* property, bool required)
		{
			auto result = parseArrayProperty<uint64_t>(json, property, required, json::value_t::number_unsigned);
			if (result) return std::vector<uint32_t>(result.value().begin(), result.value().end());
			return {};
		}

		static std::optional<std::map<std::string, float>> parseFloatObjectProperty(const json& json, const char* property, bool required)
		{
			auto result = parseObjectProperty<double>(json, property, required, std::vector { json::value_t::number_integer, json::value_t::number_unsigned, json::value_t::number_float });
			if (result) return std::map<std::string, float>(result.value().begin(), result.value().end());
			return {};
		}

		static std::optional<std::map<std::string, int32_t>> parseIntegerObjectProperty(const json& json, const char* property, bool required)
		{
			auto result = parseObjectProperty<int64_t>(json, property, required, json::value_t::number_integer);
			if (result) return std::map<std::string, int32_t>(result.value().begin(), result.value().end());
			return {};
		}

		static std::optional<std::map<std::string, uint32_t>> parseUnsignedObjectProperty(const json& json, const char* property, bool required)
		{
			auto result = parseObjectProperty<uint64_t>(json, property, required, json::value_t::number_unsigned);
			if (result) return std::map<std::string, uint32_t>(result.value().begin(), result.value().end());
			return {};
		}

	};

} // namespace Inferno
#endif

// JSON syntax:
//   [ ]        = std::map,    array (ordered)
//   { }        = std::vector, object (unordered)
//   "":        = std::string, property (key -> value)
//   true/false = bool,        condition
//   -3         = int64_t,     number
//   5          = uint64_t,    number
//   4.567      = double,      number
//   "thing"    = std::string, text
