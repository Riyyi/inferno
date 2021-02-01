#ifndef FONT_H
#define FONT_H

#include <cstdint>       // uint32_t
#include <memory>        // std::shared_ptr
#include <string>        // std::string
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include "glm/ext/vector_int2.hpp" // glm::ivec2

#include "inferno/io/log.h"

namespace Inferno {

class Texture;

	struct Character {
		glm::ivec2 position; // Position
		glm::ivec2 size;     // Width/height
		glm::ivec2 offset;   // Offset from baseline to left / top of glyph
		uint32_t advance;    // Amount to advance to next glyph
	};

// -----------------------------------------

	class Font {
	public:
		Font(const std::string& name);
		virtual ~Font() {}

		inline const std::shared_ptr<Character>& get(unsigned char c) const { return m_characterList.at(c); }
		inline const std::shared_ptr<Character>& operator[](unsigned char c) const { return m_characterList.at(c); }

		inline std::string name() const { return m_name; }
		inline const std::shared_ptr<Texture>& texture() const { return m_texture; }

	private:
		void parseFont(const std::string& font);
		const std::string findAction(const std::string& line);
		const std::vector<std::string> findColumns(const std::string& line);
		const std::string findValue(const std::string& key, const std::vector<std::string>& columns);

		std::string m_name;
		std::shared_ptr<Texture> m_texture;
		std::unordered_map<unsigned char, std::shared_ptr<Character>> m_characterList;
	};

// -----------------------------------------

	class FontManager {
	public:
		void initialize();
		void destroy();

		void add(const std::string& name, const std::shared_ptr<Font>& font);
		std::shared_ptr<Font> load(const std::string& name);
		std::shared_ptr<Font> get(const std::string& name);
		bool exists(const std::string& name);

		void remove(const std::string& name);
		void remove(const std::shared_ptr<Font>& shader);

		static inline FontManager& the() { return *s_instance; }

	private:
		std::unordered_map<std::string, std::shared_ptr<Font>> m_fontList;

		static FontManager* s_instance;
	};


// -----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const glm::ivec2& value);

}

#endif // FONT_H

// FontManager fm;
// Font f = fm.load("path/to/font");
// Font f2("path/to/font");
// Character c = f['a'];

// Look into using signed distance fields for texture map generation ? anti-aliasing for text
// https://youtu.be/d8cfgcJR9Tk
