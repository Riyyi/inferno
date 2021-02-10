#include <cstdint> // uint32_t
#include <string>  // std::string

#include "inferno/io/file.h"
#include "inferno/io/log.h"
#include "inferno/settings.h"
#include "inferno/util/json.h"

namespace Inferno {

	const char* Settings::m_path = "assets/settings.json";
	SettingsProperties Settings::m_properties = {};

	void Settings::initialize()
	{
		Settings::update();

		info() << "Settings initialized";
	}

	void Settings::update()
	{
		nlohmann::json json;
		Settings::load(json);

		if (!Json::hasProperty(json, "window")) {
			warn() << "Settings has no window section, using default values";
			return;
		}

		auto window = json["window"];
		auto title = Json::parseStringProperty(window, "title", false);
		auto width = Json::parseUnsignedProperty(window, "width", false);
		auto height = Json::parseUnsignedProperty(window, "height", false);
		auto fullscreen = Json::parseStringProperty(window, "fullscreen", false);
		auto vsync = Json::parseBoolProperty(window, "vsync", false);

		if (title)      m_properties.window.title = title.value();
		if (width)      m_properties.window.width = width.value();
		if (height)     m_properties.window.height = height.value();
		if (fullscreen) m_properties.window.fullscreen = fullscreen.value();
		if (vsync)      m_properties.window.vsync = vsync.value();
	}

	void Settings::destroy()
	{
	}

	bool Settings::load(nlohmann::json& json)
	{
		File::ioRead(json, m_path);

		return true;
	}

	bool Settings::save()
	{
		nlohmann::json json;
		json["window"]["title"]       = m_properties.window.title;
		json["window"]["width"]       = m_properties.window.width;
		json["window"]["height"]      = m_properties.window.height;
		json["window"]["fullscreen"]  = m_properties.window.fullscreen;
		json["window"]["vsync"]       = m_properties.window.vsync;

		File::ioWrite(json, m_path);
		info() << "Settings saved";

		return true;
	}

}
