/*
 * Copyright (C) 2023 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint>
#include <random>

#include "ruc/format/log.h"
#include "ruc/json/json.h"

#include "inferno/uid.h"

namespace Inferno {

static std::random_device s_seed;
static std::mt19937_64 s_engine(s_seed());
static std::uniform_int_distribution<uint64_t> s_distribution;

UID::UID()
	: m_uid(s_distribution(s_engine))
{
}

UID::UID(uint64_t uid)
	: m_uid(uid)
{
}

void fromJson(const ruc::Json& json, UID& value)
{
	VERIFY(json.type() == ruc::Json::Type::Number);

	value = UID((int64_t)json.asDouble());
}

} // namespace Inferno
