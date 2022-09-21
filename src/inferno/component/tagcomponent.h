#pragma once

#include <string>  // std::string
#include <utility> // std::move

namespace Inferno {

struct TagComponent {
	std::string tag;

	TagComponent() = default;
	TagComponent(const std::string& tag)
		: tag(std::move(tag))
	{
	}

	operator const std::string&() const { return tag; }
};

} // namespace Inferno
