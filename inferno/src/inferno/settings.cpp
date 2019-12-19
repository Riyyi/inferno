#include <fstream> // std::ifstream, std::ofstream
#include <iomanip> // std::setfill, std::setw
#include <string>  // std::string

#include "inferno/core.h"
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
		nlohmann::json m_json = this->load();

		try {
			m_properties.window.title      = strdup(m_json["window"]["title"].get<std::string>().c_str());
			m_properties.window.width      = m_json["window"]["width"].get<int>();
			m_properties.window.height     = m_json["window"]["height"].get<int>();
			m_properties.window.fullscreen = strdup(m_json["window"]["fullscreen"].get<std::string>().c_str());
			m_properties.window.vsync      = m_json["window"]["vsync"].get<bool>();
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

		std::ifstream file(m_path);
		NF_CORE_ASSERT(file.is_open(), "Could not open settings file!");

		if (file.is_open()) {
			// Read the JSON file
			file >> json;
			file.close();
		}

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

		std::ofstream file (m_path);
		NF_CORE_ASSERT(file.is_open(), "Could not open settings file!");

		if (file.is_open()) {
			// Write the JSON file with single tabs, nicely formatted
			file << std::setfill ('\t') << std::setw(1) << json << std::endl;
			file.close();
		}

		NF_CORE_INFO("Settings saved");

		return true;
	}
}
