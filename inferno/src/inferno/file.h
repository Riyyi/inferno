#ifndef FILE_H
#define FILE_H

#include <fstream> // std::ifstream, std::ofstream
#include <iomanip> // std::setfill, std::setw
#include <string>  // std::string

#include "inferno/core.h"
#include "inferno/log.h"

namespace Inferno {

	class File {
	public:
		static std::string read(const std::string &path);

		template<typename T>
		static void ioRead(T &t, const std::string &path)
		{
			std::ifstream file(path);
			NF_CORE_ASSERT(file.is_open(), "File could not open: %s", path.c_str());

			if (file.is_open()) {
				file >> t;
				file.close();
			}
		}

		template<typename T>
		static void ioWrite(T &t, const std::string &path)
		{
			std::ofstream file (path);
			NF_CORE_ASSERT(file.is_open(), "File could not open: %s", path.c_str());

			if (file.is_open()) {
				// Write file with single tabs, nicely formatted
				file << std::setfill ('\t') << std::setw(1) << t << std::endl;
				file.close();
			}
		}
	};

}

#endif // FILE_H
