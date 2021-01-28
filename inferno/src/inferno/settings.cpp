#include <string> // std::string

#include "inferno/io/file.h"
#include "inferno/io/log.h"
#include "inferno/settings.h"

namespace Inferno {

	const char* Settings::m_path = "assets/settings.json";
	SettingsProperties Settings::m_properties = {};

	void Settings::initialize()
	{
		Settings::update();

		dbg(Log::Info) << "Settings initialized";
	}

	void Settings::update()
	{
		nlohmann::json json;
		Settings::load(json);

		try {
			m_properties.window.title       = json["window"]["title"].get<std::string>();
			m_properties.window.width       = json["window"]["width"].get<int>();
			m_properties.window.height      = json["window"]["height"].get<int>();
			m_properties.window.fullscreen  = json["window"]["fullscreen"].get<std::string>();
			m_properties.window.vsync       = json["window"]["vsync"].get<bool>();
		}
		catch (...) {
			dbg(Log::Warn) << "Settings syntax error: using default values";
		}
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
		dbg(Log::Info) << "Settings saved";

		return true;
	}

}
