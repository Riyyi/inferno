#include <bits/stdint-uintn.h>
#include <string> // std::getline

#include "inferno/assertions.h"
#include "inferno/file.h"
#include "inferno/render/font.h"
#include "inferno/render/texture.h"

namespace Inferno {

	Font::Font(const std::string& name)
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

			if (findAction(line).compare("char") != 0) {
				continue;
			}

			const std::vector<std::string> data = findColumns(line);

			unsigned char id = std::stoi(findValue("id", data));
			Character character = {
				{ std::stoi(findValue("x", data)), std::stoi(findValue("y", data)) },
				{ std::stoi(findValue("width", data)), std::stoi(findValue("height", data)) },
				{ std::stoi(findValue("xoffset", data)), std::stoi(findValue("yoffset", data)) },
				std::stoi(findValue("xadvance", data))
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
			find           = line.find(" ", index);
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

		return elements;
	}

	const std::string Font::findValue(const std::string& key, const std::vector<std::string>& columns)
	{
		size_t find = 0;
		// Loop over columns
		for (auto it = columns.begin(); it != columns.end(); it++) {
			find = it->find(key + "=");
			if (find != std::string::npos) {
				return it->substr(key.length() + 1);
			}
		}

		return "";
	}

// -----------------------------------------

	FontManager* FontManager::s_instance = nullptr;

	void FontManager::initialize()
	{
		ASSERT(!s_instance, "FontManager already exists!");
		s_instance = this;

		dbg(Log::Info) << "FontManager initialized";
	}

	void FontManager::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	void FontManager::add(const std::string& name, const std::shared_ptr<Font>& font)
	{
		// Construct (key, value) pair and insert it into the unordered_map
		m_fontList.emplace(name, font);
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

	void FontManager::remove(const std::shared_ptr<Font>& font)
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

}