/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint64_t

#include "ruc/json/json.h"

namespace Inferno {

class UID {
public:
	UID();
	UID(uint64_t uid);

	// Comparison operator for std::map
	bool operator<(const UID& other) const
	{
		return m_uid < other.m_uid;
	}

	operator uint64_t() const { return m_uid; }

private:
	uint64_t m_uid;
};

void fromJson(const ruc::Json& json, UID& value);

} // namespace Inferno

namespace std {

// Hash function for std::unordered_map
template<>
struct hash<Inferno::UID> {
	size_t operator()(const Inferno::UID& uid) const
	{
		return hash<uint64_t>()(static_cast<uint64_t>(uid));
	}
};

} // namespace std
