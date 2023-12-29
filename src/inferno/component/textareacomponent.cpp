/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "ruc/json/json.h"

#include "inferno/component/textareacomponent.h"

namespace Inferno {

void fromJson(const ruc::Json& json, TextAreaComponent& value)
{
	VERIFY(json.type() == ruc::Json::Type::Object);

	if (json.exists("content") && json.at("content").type() == ruc::Json::Type::String) {
		json.at("content").getTo(value.content);
	}
	if (json.exists("font") && json.at("font").type() == ruc::Json::Type::String) {
		json.at("font").getTo(value.font);
	}
	if (json.exists("font-size") && json.at("font-size").type() == ruc::Json::Type::Number) {
		json.at("font-size").getTo(value.fontSize);
	}
	if (json.exists("line-spacing") && json.at("line-spacing").type() == ruc::Json::Type::Number) {
		json.at("line-spacing").getTo(value.lineSpacing);
	}
	if (json.exists("width") && json.at("width").type() == ruc::Json::Type::Number) {
		json.at("width").getTo(value.width);
	}
}

} // namespace Inferno
