#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include <string> // std::string

namespace Inferno {

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator const std::string&() const { return tag; }
	};

}

#endif // TAG_COMPONENT_H
