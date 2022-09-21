#include <ios>    // std::ios
#include <memory> // std::make_unique

#include "ruc/meta/assert.h"

#include "inferno/io/file.h"

namespace Inferno {

std::shared_ptr<char[]> File::raw(const std::string& path)
{
	// Create input stream object and open file
	std::ifstream file(path);
	VERIFY(file.is_open(), "File could not open '{}'", path);

	// Get length of the file
	int32_t length = File::length(path, file);

	// Allocate memory filled with zeros
	auto buffer = std::shared_ptr<char[]>(new char[length + 1]);

	// Fill buffer with file contents
	file.read(buffer.get(), length);
	file.close();

	// Null termination
	buffer[length] = '\0';

	return buffer;
}

std::string File::read(const std::string& path)
{
	// Create string from the buffer and return
	return std::string(raw(path).get());
}

int32_t File::length(const std::string& path, std::ifstream& file)
{
	file.seekg(0, std::ios::end);
	int32_t length = file.tellg();
	file.seekg(0, std::ios::beg);
	VERIFY(length != -1, "File could not determine length '{}'", path);

	return length;
}

} // namespace Inferno
