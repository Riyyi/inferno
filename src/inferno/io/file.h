#pragma once

#include <fstream> // std::ifstream, std::ofstream
#include <iomanip> // std::setfill, std::setw
#include <memory>  // std::shared_ptr
#include <string>  // std::string

#include "inferno/core.h"
#include "inferno/io/log.h"
#include "ruc/meta/assert.h"

namespace Inferno {

	class File {
	public:
		static std::shared_ptr<char[]> raw(const std::string& path);
		static std::string read(const std::string& path);
		static int32_t length(const std::string& path, std::ifstream& file);

		template<typename T>
		static bool ioRead(T* t, const std::string& path)
		{
			std::ifstream file(path);
			VERIFY(file.is_open(), "File could not open '{}'", path);

			if (!file.is_open()) {
				return false;
			}

			try {
				file >> *t;
			}
			catch (...) {
				return false;
			}

			return true;
		}

		template<typename T>
		static bool ioWrite(T* t, const std::string& path)
		{
			std::ofstream file (path);
			VERIFY(file.is_open(), "File could not open! {}", path);

			if (!file.is_open()) {
				return false;
			}

			try {
				// Write file with single tabs, nicely formatted
				file << std::setfill ('\t') << std::setw(1) << *t << std::endl;
			}
			catch (...) {
				return false;
			}

			return true;
		}
	};

} // namespace Inferno
