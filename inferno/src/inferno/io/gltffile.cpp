#include <cstring> // strcmp ??
#include <fstream> // std::ifstream
#include <ios>     // std::ios

#include "inferno/assert.h"
#include "inferno/io/file.h"
#include "inferno/io/gltffile.h"
#include "inferno/io/log.h"
#include "inferno/util/string.h"
#include "inferno/util/json.h"

namespace Inferno {

	std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> GltfFile::read(const std::string& path)
	{
		std::string extension = path.substr(path.find_first_of("."));

		if (extension.compare(".glb") == 0) {
			return glb(path);
		}
		else if (extension.compare(".gltf") == 0) {
			return gltf(path);
		}

		ASSERT(false, "GltfFile unknown file extension!");
		return {};
	}

	std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> GltfFile::glb(const std::string& path)
	{
		// Create input stream object and open file
		std::ifstream glb(path, std::ios::in | std::ios::binary);
		ASSERT(glb.is_open(), "GltfFile could not open '{}'", path);

		constexpr uint32_t size = 4;
		constexpr uint32_t header = 12;
		constexpr uint32_t json = 27; // Minimum valid glTF has an asset property with version specifier

		// Get the actual length of the file
		uint32_t length = static_cast<uint32_t>(File::length(path, glb));
		ASSERT(length > header + json, "GltfFile too small to be valid '{}'", path);

		// Read header

		char magic[size];
		char version[size];
		char fileLength[size];

		glb.read(magic, size);
		glb.seekg(size * 1);
		glb.read(version, size);
		glb.seekg(size * 2);
		glb.read(fileLength, size);

		// Validate header

		uint32_t magicInt = *reinterpret_cast<uint32_t*>(magic);
		ASSERT(magicInt == 0x46546c67, "Gltf invalid header magic '{}'", magic);

		uint32_t versionInt = *reinterpret_cast<uint32_t*>(version);
		ASSERT(versionInt == 2, "Gltf unsupported version '{}'", versionInt);

		uint32_t fileLengthInt = *reinterpret_cast<uint32_t*>(fileLength);
		ASSERT(fileLengthInt == length, "Gltf file and reported byte size mismatch '{}' '{}'", length, fileLengthInt);

		// Read JSON data
		auto jsonChunk = readChunk(glb, header, 0x4e4f534a);
		ASSERT(jsonChunk.second >= json, "Gltf file invalid JSON content length '{}'", jsonChunk.second);

		// Read binary data
		auto binaryChunk = readChunk(glb, header + size * 2 + jsonChunk.second, 0x004e4942);

		glb.close();

		return { jsonChunk.first, binaryChunk.first };
	}

	std::pair<std::shared_ptr<char[]>, uint32_t> GltfFile::readChunk(std::ifstream& ifstream, uint32_t offset, uint32_t type)
	{
		constexpr uint32_t size = 4;

		char chunkLength[size];
		char chunkType[size];

		ifstream.seekg(offset);
		ifstream.read(chunkLength, size);
		ifstream.seekg(offset + size * 1);
		ifstream.read(chunkType, size);

		uint32_t chunkTypeInt = *reinterpret_cast<uint32_t*>(chunkType);
		ASSERT(chunkTypeInt == type, "Gltf invalid chunk type '{}' != '{}'", chunkType, intToHex(type));

		uint32_t chunkLengthInt = *reinterpret_cast<uint32_t*>(chunkLength);
		// Allocate memory filled with zeros
		std::shared_ptr<char[]> chunkData(new char[chunkLengthInt + 1]);
		ifstream.seekg(offset + size * 2);
		ifstream.read(chunkData.get(), chunkLengthInt);
		chunkData.get()[chunkLengthInt] = '\0';

		return { chunkData, chunkLengthInt };
	}

	std::pair<std::shared_ptr<char[]>, std::shared_ptr<char[]>> GltfFile::gltf(const std::string& path)
	{
		auto jsonContent = File::raw(path);

		json json = json::parse(jsonContent.get());
		ASSERT(Json::hasProperty(json, "buffers"), "GltfFile missing required property 'buffers'");

		std::shared_ptr<char[]> binaryContent { nullptr };
		for (auto& [key, buffer] : json["buffers"].items()) {
			auto uri = Json::parseStringProperty(buffer, "uri", true);
			ASSERT(uri, "GltfFile missing required property 'uri'");
			ASSERT(uri.value().find("data:", 0) == std::string::npos, "GltfFile embedded binary data is unsupported");
			binaryContent = File::raw("assets/gltf/" + uri.value());
			break;
		}

		return { jsonContent, binaryContent };
	}

} // namespace Inferno
