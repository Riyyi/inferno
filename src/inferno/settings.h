#ifndef SETTINGS_H
#define SETTINGS_H

#include "inferno/util/json.h"
#include "inferno/window.h"

namespace Inferno {

	struct SettingsProperties {
		WindowProperties window;
	};

	class Settings {
	public:
		static void initialize();
		static void destroy();

		static bool load();
		static bool save();

		static inline SettingsProperties& get() { return m_properties; }

	private:
		static const char* m_path;
		static SettingsProperties m_properties;
	};

// -----------------------------------------

	// nlohmann::json arbitrary type conversion functions

	void to_json(json& object, const SettingsProperties& settings);
	void from_json(const json& object, SettingsProperties& settings);

	void to_json(json& object, const WindowProperties& window);
	void from_json(const json& object, WindowProperties& window);

}

#endif // SETTINGS_H
