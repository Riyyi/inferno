#include <string>  // std::string

#include "inferno/file.h"
#include "inferno/log.h"
#include "inferno/settings.h"

namespace Inferno {

	bool Settings::m_initialized = false;

	const char* Settings::m_path = "assets/settings.json";
	SettingsProperties Settings::m_properties = {};

	void Settings::initialize()
	{
		Settings::update();

		m_initialized = true;
	}

	void Settings::update()
	{
		if (m_initialized) {
			Settings::destroy();
		}

		nlohmann::json json = Settings::load();

		try {
			m_properties.window.title       = strdup(json["window"]["title"].get<std::string>().c_str());
			m_properties.window.width       = json["window"]["width"].get<int>();
			m_properties.window.height      = json["window"]["height"].get<int>();
			m_properties.window.fullscreen  = strdup(json["window"]["fullscreen"].get<std::string>().c_str()) ;
			m_properties.window.vsync       = json["window"]["vsync"].get<bool>();
		}
		catch (...) {
			NF_CORE_WARN("Settings syntax error: using default values");
		}
	}

	void Settings::destroy()
	{
		// Delete const char*s created by strdup()
		delete m_properties.window.title;
		delete m_properties.window.fullscreen;
	}

	nlohmann::json Settings::load()
	{
		nlohmann::json json;

		File::ioRead(json, m_path);
		NF_CORE_INFO("Settings loaded");

		return json;
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
		NF_CORE_INFO("Settings saved");

		return true;
	}

}
