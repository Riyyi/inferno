#include <iostream> // std::ios
#include <memory>   // std::make_unique

#include "inferno/assertions.h"
#include "inferno/file.h"

namespace Inferno {

	std::string File::read(const std::string &path)
	{
		// Create input stream object and open file
		std::ifstream file(path.c_str());
		ASSERT(file.is_open(), "File could not open! {}", path.c_str());

		// Check if file exists
		if (!file.is_open()) {
			return nullptr;
		}

		// Get length of the file
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		file.seekg(0, std::ios::beg);
		ASSERT(length != -1, "File could not determine length! {}", path.c_str());

		// Check for valid file length
		if (length == -1) {
			return nullptr;
		}

		// Allocate memory filled with zeros
		auto buffer = std::make_unique<char[]>(length + 1);

		// Fill buffer with file contents
		file.read(buffer.get(), length);
		file.close();

		// Create string from the buffer and return
		return std::string(buffer.get(), length + 1);
	}

}
