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
			m_properties.title = m_json["window"]["title"].get<std::string>().c_str();
			m_properties.width = m_json["window"]["width"].get<int>();
			m_properties.height = m_json["window"]["height"].get<int>();
		}
		catch (...) {
			NF_CORE_WARN("Settings syntax error: using default values");
		}
	}

	void Settings::destroy()
	{
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
		json["window"]["title"] = m_properties.title;
		json["window"]["width"] = m_properties.width;
		json["window"]["height"] = m_properties.height;

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
