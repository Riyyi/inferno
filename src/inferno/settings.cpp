#include <cstdint> // uint32_t
#include <string>  // std::string

#include "ruc/json/json.h"

#include "inferno/io/file.h"
#include "inferno/io/log.h"
#include "inferno/settings.h"
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
	ruc::Json object;

	if (!File::ioRead(&object, m_path)) {
		warn() << "Settings invalid formatting, using default values";
		return false;
	}

	m_properties = object.get<SettingsProperties>();

	return true;
}

bool Settings::save()
{
	ruc::Json object = m_properties;

	if (!File::ioWrite(&object, m_path)) {
		warn() << "Settings could not be saved";
		return false;
	}

	info() << "Settings saved";
	return true;
}

// -------------------------------------

void toJson(ruc::Json& object, const SettingsProperties& settings)
{
	object = ruc::Json {
		{ "window", settings.window }
	};
}

void fromJson(const ruc::Json& object, SettingsProperties& settings)
{
	VERIFY(object.type() == ruc::Json::Type::Object);

	if (object.exists("window"))
		object.at("window").getTo(settings.window);
}

void toJson(ruc::Json& object, const WindowProperties& window)
{
	object = ruc::Json {
		{ "title", window.title },
		{ "width", window.width },
		{ "height", window.height },
		{ "fullscreen", window.fullscreen },
		{ "vsync", window.vsync },
	};
}

void fromJson(const ruc::Json& object, WindowProperties& window)
{
	VERIFY(object.type() == ruc::Json::Type::Object);

	if (object.exists("title"))
		object.at("title").getTo(window.title);
	if (object.exists("width"))
		object.at("width").getTo(window.width);
	if (object.exists("height"))
		object.at("height").getTo(window.height);
	if (object.exists("fullscreen"))
		object.at("fullscreen").getTo(window.fullscreen);
	if (object.exists("vsync"))
		object.at("vsync").getTo(window.vsync);
}

} // namespace Inferno
