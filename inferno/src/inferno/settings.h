#ifndef SETTINGS_H
#define SETTINGS_H

#include <nlohmann/json.hpp>

namespace Inferno {

	struct SettingsProperties {
		const char* title = "Inferno default";
		int width = 1280;
		int height = 720;
	};

	class Settings {
	public:
		Settings(const char* path = "assets/settings.json");
		virtual ~Settings();

		void initialize();
		void destroy();

		nlohmann::json load() const;
		bool save();

// -----------------------------------------

		static inline Settings &get() { return *s_instance; }

		inline SettingsProperties &properties() { return m_properties; }

	private:
		const char* m_path;
		SettingsProperties m_properties;

		static Settings* s_instance;
	};

}

#endif // SETTINGS_H