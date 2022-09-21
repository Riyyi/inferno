#pragma once

#include <iomanip> // std::setfill, std::setw
#include <sstream> // std::stringstream

namespace Inferno {

template<typename T>
std::string intToHex(T i)
{
	std::stringstream stream;
	stream << "0x"
		   << std::setfill('0') << std::setw(sizeof(T) * 2)
		   << std::hex << i;

	return stream.str();
}

} // namespace Inferno
