#ifndef GLTF_FILE_H
#define GLTF_FILE_H

#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr
#include <string>  // std::string
#include <utility> // std::pair

namespace Inferno {

	class GltfFile {
	public:
		static std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> read(const std::string& path);

	private:
		static std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> glb(const std::string& path);
		static std::pair<std::shared_ptr<char[]>, uint32_t> readChunk(std::ifstream& ifstream, uint32_t offset, uint32_t type);

		static std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> gltf(const std::string& path);
	};

} // namespace Inferno

#endif // GLTF_FILE_H
