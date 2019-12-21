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
		Settings(const char* path = "assets/settings.json");
		virtual ~Settings();

// -----------------------------------------

		void initialize();
		void update();
		// void render();
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
