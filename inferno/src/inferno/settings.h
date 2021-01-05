#ifndef SETTINGS_H
#define SETTINGS_H

#include <nlohmann/json.hpp>

#include "inferno/window.h"

namespace Inferno {

	struct SettingsProperties {
		WindowProperties window;
	};

	class Settings {
	public:
		static void initialize();
		static void update();
		static void destroy();

		static nlohmann::json load();
		static bool save();

		static inline SettingsProperties& get() { return m_properties; }

	private:
		static const char* m_path;
		static SettingsProperties m_properties;
	};

}

#endif // SETTINGS_H
