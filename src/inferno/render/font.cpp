#include <limits>  // std::numeric_limits
#include <string>  // std::getline, std::stoi
#include <utility> // std::move

#include "ruc/meta/assert.h"

#include "inferno/io/file.h"
#include "inferno/render/font.h"
#include "inferno/render/texture.h"
#include "inferno/util/integer.h"

namespace Inferno {

Font::Font(const std::string& name)
	: m_name(std::move(name))
{
	std::string path = name + ".fnt";
	std::string image = name + ".png";

	std::string font = File::read(path);
	parseFont(font);

	m_texture = std::make_shared<Texture>(image);
}

void Font::parseFont(const std::string& font)
{
	std::istringstream iss(font);
	for (std::string line; std::getline(iss, line);) {

		if (findAction(line).compare("info") != 0 && findAction(line).compare("char") != 0) {
			continue;
		}

		const std::vector<std::string> columns = findColumns(line);

		// Info

		if (findAction(line).compare("info") == 0) {
			m_size = std::stou(findValue("size", columns));
			continue;
		}

		// Character

		unsigned char id = std::stoi(findValue("id", columns));
		Character character = {
			{ std::stou(findValue("x", columns)), std::stou(findValue("y", columns)) },
			{ std::stou(findValue("width", columns)), std::stou(findValue("height", columns)) },
			{ std::stoi(findValue("xoffset", columns)), std::stoi(findValue("yoffset", columns)) },
			std::stou(findValue("xadvance", columns))
		};
		m_characterList.emplace(id, std::make_shared<Character>(character));
	}
}

const std::string Font::findAction(const std::string& line)
{
	return line.substr(0, line.find(" "));
}

const std::vector<std::string> Font::findColumns(const std::string& line)
{
	std::vector<std::string> elements;

	size_t index = 0;
	size_t find = 0;
	size_t findFirstNotOf = 0;
	// Loop over line characters
	while (find != std::string::npos) {
		find = line.find(" ", index);
		findFirstNotOf = line.find_first_not_of(" ", index);

		// Add element
		if (find > findFirstNotOf) {
			elements.push_back(line.substr(index, find - findFirstNotOf));
			index += 1 + find - findFirstNotOf;
		}
		// Skip double spaces
		else {
			index = findFirstNotOf;
		}
	}

	VERIFY(!elements.empty(), "Font file did not find any columns");
	return elements;
}

const std::string Font::findValue(const std::string& key, const std::vector<std::string>& columns)
{
	size_t find = 0;
	// Loop over columns
	for (auto& column : columns) {
		find = column.find(key + "=");
		if (find != std::string::npos) {
			return column.substr(key.length() + 1);
		}
	}

	VERIFY(false, "Font file did not contain key '{}'", key);
	return "";
}

// -----------------------------------------

FontManager::FontManager(s)
{
	info() << "FontManager initialized";
}

FontManager::~FontManager()
{
}

void FontManager::add(const std::string& name, std::shared_ptr<Font> font)
{
	// Construct (key, value) pair and insert it into the unordered_map
	m_fontList.emplace(std::move(name), std::move(font));
}

std::shared_ptr<Font> FontManager::load(const std::string& name)
{
	if (exists(name)) {
		return get(name);
	}

	std::shared_ptr<Font> font = std::make_shared<Font>(name);
	add(name, font);
	return get(name);
}

std::shared_ptr<Font> FontManager::get(const std::string& name)
{
	return exists(name) ? m_fontList.at(name) : nullptr;
}

bool FontManager::exists(const std::string& name)
{
	return m_fontList.find(name) != m_fontList.end();
}

void FontManager::remove(const std::string& name)
{
	if (exists(name)) {
		m_fontList.erase(name);
	}
}

void FontManager::remove(std::shared_ptr<Font> font)
{
	if (exists(font->name())) {
		m_fontList.erase(font->name());
	}
}

// -----------------------------------------

const LogStream& operator<<(const LogStream& stream, const glm::ivec2& value)
{
	return stream << "{ " << value.x << ", " << value.y << " }";
}

} // namespace Inferno
