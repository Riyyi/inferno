#include <iostream> // std::ios
#include <memory>   // std::make_unique

#include "inferno/assert.h"
#include "inferno/io/file.h"

namespace Inferno {

	std::string File::read(const std::string &path)
	{
		// Create input stream object and open file
		std::ifstream ifstream(path);
		ASSERT(ifstream.is_open(), "File could not open '{}'", path);

		// Check if file exists
		if (!ifstream.is_open()) {
			return nullptr;
		}

		// Get length of the file
		int32_t length = File::length(path, ifstream);

		// Check for valid file length
		if (length == -1) {
			return nullptr;
		}

		// Allocate memory filled with zeros
		auto buffer = std::make_unique<char[]>(length);

		// Fill buffer with file contents
		ifstream.read(buffer.get(), length);
		ifstream.close();

		// Create string from the buffer and return
		return std::string(buffer.get(), length);
	}

	int32_t File::length(const std::string& path, std::ifstream& ifstream)
	{
		ifstream.seekg(0, std::ios::end);
		int32_t length = ifstream.tellg();
		ifstream.seekg(0, std::ios::beg);
		ASSERT(length != -1, "File could not determine length '{}'", path);

		return length;
	}

}
