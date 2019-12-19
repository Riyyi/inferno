#include <fstream> // std::ifstream, std::ofstream
#include <iomanip> // std::setfill, std::setw
#include <string>  // std::string

#include "inferno/core.h"
#include "inferno/file.h"
#include "inferno/log.h"
#include "inferno/settings.h"

namespace Inferno {

	Settings* Settings::s_instance = nullptr;

	Settings::Settings(const char* path) :
		m_path(path)
	{
		NF_CORE_ASSERT(!s_instance, "Settings already exists!");
		s_instance = this;

		this->initialize();
	}

	Settings::~Settings()
	{
		this->destroy();
	}

	void Settings::initialize()
	{
		nlohmann::json json = this->load();

		try {
			m_properties.window.title      = strdup(json["window"]["title"].get<std::string>().c_str());
			m_properties.window.width      = json["window"]["width"].get<int>();
			m_properties.window.height     = json["window"]["height"].get<int>();
			m_properties.window.fullscreen = strdup(json["window"]["fullscreen"].get<std::string>().c_str());
			m_properties.window.vsync      = json["window"]["vsync"].get<bool>();
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

	nlohmann::json Settings::load() const
	{
		nlohmann::json json;

		File::ioRead(json, m_path);
		NF_CORE_INFO("Settings loaded");

		return json;
	}

	bool Settings::save()
	{
		nlohmann::json json;
		json["window"]["title"]      = m_properties.window.title;
		json["window"]["width"]      = m_properties.window.width;
		json["window"]["height"]     = m_properties.window.height;
		json["window"]["fullscreen"] = m_properties.window.fullscreen;
		json["window"]["vsync"]      = m_properties.window.vsync;

		File::ioWrite(json, m_path);
		NF_CORE_INFO("Settings saved");

		return true;
	}
}
