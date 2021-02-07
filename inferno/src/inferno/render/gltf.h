#ifndef GLTF_H
#define GLTF_H

#include <cstdint>       // uint32_t
#include <memory>        // std::shared_ptr
#include <string>        // std::string
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include "inferno/util/json.h"

namespace Inferno {

	namespace glTF {

		// Type specifications
		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#objects

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#asset
		struct Asset {
			std::string copyright;
			std::string generator;
			std::string version; // Required
			std::string minVersion;
		};

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#scenes
		struct Scene {
			std::vector<uint32_t> nodes;
			std::string name;
		};

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#nodes-and-hierarchy
		struct Node {
			uint32_t camera;
			std::vector<uint32_t> children;
			uint32_t skin;
			std::array<double, 16> matrix { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; // Identity matrix
			uint32_t mesh;
			std::array<double, 4> rotation { 0, 0, 0, 1 };
			std::array<double, 3> scale { 1, 1, 1 };
			std::array<double, 3> translation { 0, 0, 0 };
			std::vector<double> weights;
			std::string name;
		};

		struct Primitive {
			std::map<std::string, uint32_t> attributes; // Required
			uint32_t indices;
			uint32_t material;
			unsigned char mode { 4 };
			std::vector<std::map<std::string, uint32_t>> targets;
		};

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
		struct Mesh {
			std::vector<Primitive> primitives; // Required
			std::vector<double> weights;
			std::string name;
		};

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#accessors
		struct Accessor {
			uint32_t bufferView;
			uint32_t byteOffset { 0 };
			uint32_t componentType;    // Required
			bool normalized { false };
			uint32_t count;            // Required
			std::string type;          // Required
			std::vector<double> max;
			std::vector<double> min;
			std::string name;
		};

		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#buffers-and-buffer-views
		struct BufferView {
			uint32_t buffer;           // Required
			uint32_t byteOffset { 0 };
			uint32_t byteLength;       // Required
			uint32_t byteStride;
			uint32_t target;
			std::string name;
		};

		struct Buffer {
			std::string uri;
			uint32_t byteLength; // Required
			std::string name;
		};

		struct Model {
			Asset asset;

			std::vector<Scene> scenes;
			std::vector<Node> nodes;
			std::vector<Mesh> meshes;
			std::vector<Accessor> accessors;
			std::vector<BufferView> bufferViews;
			std::vector<Buffer> buffers;
		};

	} // namespace glTF

// -----------------------------------------

	class Gltf {
	public:
		Gltf(const std::string& path);
		virtual ~Gltf();

		inline const glTF::Model& model() const { return m_model; }

	private:
		static void parseAsset(glTF::Asset* asset, const json& object);
		static void parseScene(glTF::Scene* scene, const std::string& key, const json& object);
		static void parseNode(glTF::Node* node, const std::string& key, const json& object);
		static void parsePrimitive(glTF::Primitive* primitive, const std::string& key, const json& object);
		static void parseMesh(glTF::Mesh* mesh, const std::string& key, const json& object);
		static void parseAccessor(glTF::Accessor* accessor, const std::string& key, const json& object);
		static void parseBufferView(glTF::BufferView* bufferView, const std::string& key, const json& object);
		static void parseBuffer(glTF::Buffer* buffer, const std::string& key, const json& object);

		std::string m_path;
		glTF::Model m_model;
	};

// -----------------------------------------

	class GltfManager {
	public:
		void initialize();
		void destroy();

		void add(const std::string& path, const std::shared_ptr<Gltf>& gltf);
		std::shared_ptr<Gltf> load(const std::string& path);
		std::shared_ptr<Gltf> get(const std::string& path);
		bool exists(const std::string& path);

		void remove(const std::string& path);
		void remove(const std::shared_ptr<Gltf>& gltf);

		static inline GltfManager& the() { return *s_instance; }

	private:
		std::unordered_map<std::string, std::shared_ptr<Gltf>> m_gltfList;

		static GltfManager* s_instance;
	};

} // namespace Inferno


#endif // GLTF_H
