#include <cstdint> // uint32_t
#include <string>  // std::string

#include "inferno/io/file.h"
#include "inferno/io/log.h"
#include "inferno/settings.h"
#include "inferno/util/json.h"
#include "inferno/window.h"

namespace Inferno {

	const char* Settings::m_path { "assets/settings.json" };
	SettingsProperties Settings::m_properties {};

	void Settings::initialize()
	{
		Settings::load();

		info() << "Settings initialized";

		Settings::save();
	}

	void Settings::destroy()
	{
	}

	bool Settings::load()
	{
		json object;

		if (!File::ioRead(&object, m_path)) {
			warn() << "Settings invalid formatting, using default values";
			return false;
		}

		auto settings = Json::getPropertyValue<SettingsProperties>(object, json::value_t::object);
		if (settings) {
			m_properties = settings.value();
		}

		return true;
	}

	bool Settings::save()
	{
		json object = m_properties;
		if (!File::ioWrite(&object, m_path)) {
			warn() << "Settings could not be saved";
			return false;
		}

		info() << "Settings saved";
		return true;
	}

// -----------------------------------------

	void to_json(json& object, const SettingsProperties& settings)
	{
		object = json {
			{ "window", settings.window }
		};
	}

	void from_json(const json& object, SettingsProperties& settings)
	{
		if (Json::hasProperty(object, "window")) object.at("window").get_to(settings.window);
	}

	void to_json(json& object, const WindowProperties& window)
	{
		object = json {
			{ "title", window.title },
			{ "width", window.width },
			{ "height", window.height },
			{ "fullscreen", window.fullscreen },
			{ "vsync", window.vsync },
		};
	}

	void from_json(const json& object, WindowProperties& window)
	{
		if (Json::hasProperty(object, "title"))      object.at("title").get_to(window.title);
		if (Json::hasProperty(object, "width"))      object.at("width").get_to(window.width);
		if (Json::hasProperty(object, "height"))     object.at("height").get_to(window.height);
		if (Json::hasProperty(object, "fullscreen")) object.at("fullscreen").get_to(window.fullscreen);
		if (Json::hasProperty(object, "vsync"))      object.at("vsync").get_to(window.vsync);
	}

}
