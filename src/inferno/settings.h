#pragma once

#include "ruc/json/json.h"

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

	// Json arbitrary type conversion functions

	void toJson(ruc::Json& object, const SettingsProperties& settings);
	void fromJson(const ruc::Json& object, SettingsProperties& settings);

	void toJson(ruc::Json& object, const WindowProperties& window);
	void fromJson(const ruc::Json& object, WindowProperties& window);

} // namespace Inferno
