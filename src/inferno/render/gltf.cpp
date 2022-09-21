#if 0
#include <algorithm> // std::copy
#include <utility>   // std::move

#include "nlohmann/json.hpp"
#include "ruc/meta/assert.h"

#include "inferno/io/file.h"
#include "inferno/io/gltffile.h"
#include "inferno/io/log.h"
#include "inferno/render/gltf.h"
#include "inferno/util/integer.h"

namespace Inferno {

	Gltf::Gltf(const std::string& path)
		: m_path(std::move(path))
	{
		auto gltf = GltfFile::read(path);
		VERIFY(gltf.first, "Gltf model invalid JSON content '{}'", path);

		json json = json::parse(gltf.first.get());

		// Add binary data from .glb files
		if (gltf.second) {
			m_model.data.emplace(0, std::move(gltf.second));
		}

		// Properties

		// Asset
		// ---------------------------------

		VERIFY(Json::hasProperty(json, "asset"), "GlTF model missing required property 'asset'");
		auto asset = json["asset"];
		VERIFY(asset.is_object(), "Gltf model invalid property type 'asset'");

		parseAsset(&m_model.asset, asset);

		// Scene
		// ---------------------------------

		if (Json::hasProperty(json, "scenes")) {

			auto scenes = json["scenes"];
			VERIFY(scenes.is_array(), "Gltf model invalid property type 'scenes'");

			for (auto& [key, object] : scenes.items()) {
				glTF::Scene scene;
				parseScene(&scene, key, object);
				m_model.scenes.emplace_back(std::move(scene));
			}
		}

		// Node
		// ---------------------------------

		if (Json::hasProperty(json, "nodes")) {

			auto nodes = json["nodes"];
			VERIFY(nodes.is_array(), "Gltf model invalid property type 'nodes'");

			for (auto& [key, object] : nodes.items()) {
				glTF::Node node;
				parseNode(&node, key, object);
				m_model.nodes.emplace_back(std::move(node));
			}
		}

		// Mesh
		// ---------------------------------

		if (Json::hasProperty(json, "meshes")) {

			auto meshes = json["meshes"];
			VERIFY(meshes.is_array(), "Gltf model invalid property type 'meshes'");

			for (auto& [key, object] : meshes.items()) {
				glTF::Mesh mesh;
				parseMesh(&mesh, key, object);
				m_model.meshes.emplace_back(std::move(mesh));
			}
		}

		// Accessor
		// ---------------------------------

		if (Json::hasProperty(json, "accessors")) {

			auto accessors = json["accessors"];
			VERIFY(accessors.is_array(), "Gltf model invalid property type 'accessors'");

			for (auto& [key, object] : accessors.items()) {
				glTF::Accessor accessor;
				parseAccessor(&accessor, key, object);
				m_model.accessors.emplace_back(std::move(accessor));
			}
		}

		// Bufferview
		// ---------------------------------

		if (Json::hasProperty(json, "bufferViews")) {

			auto bufferViews = json["bufferViews"];
			VERIFY(bufferViews.is_array(), "Gltf model invalid property type 'bufferViews'");

			for (auto& [key, object] : bufferViews.items()) {
				glTF::BufferView bufferView;
				parseBufferView(&bufferView, key, object);
				m_model.bufferViews.emplace_back(std::move(bufferView));
			}
		}

		// Buffer
		// ---------------------------------

		if (Json::hasProperty(json, "buffers")) {

			auto buffers = json["buffers"];
			VERIFY(buffers.is_array(), "Gltf model invalid property type 'buffers'");

			for (auto& [key, object] : buffers.items()) {
				glTF::Buffer buffer;
				parseBuffer(&buffer, key, object, &m_model.data);
				m_model.buffers.emplace_back(std::move(buffer));
			}
		}

	}

	Gltf::~Gltf()
	{
	}

	void Gltf::parseAsset(glTF::Asset* asset, const json& object)
	{
		auto copyright = Json::parseStringProperty(object, "copyright", false);

		auto generator = Json::parseStringProperty(object, "generator", false);

		auto version = Json::parseStringProperty(object, "version", true);
		VERIFY(version, "GlTF model missing required property 'version'");
		VERIFY(version.value().compare("2.0") == 0, "GlTF version unsupported '{}'", version.value());

		auto minVersion = Json::parseStringProperty(object, "minVersion", false);

		if (copyright)  asset->copyright = copyright.value();
		if (generator)  asset->generator = generator.value();
		if (version)    asset->version = version.value();
		if (minVersion) asset->minVersion = minVersion.value();
	}

	void Gltf::parseScene(glTF::Scene* scene, const std::string& key, const json& object)
	{
		auto nodes = Json::parseFloatArrayProperty(object, "nodes", false);
		VERIFY(!nodes || nodes.value().size() > 0, "Gltf scene '{}' empty 'nodes' property", key);

		auto name = Json::parseStringProperty(object, "name", false);

		scene->name = name ? name.value() : key;
	}

	void Gltf::parseNode(glTF::Node* node, const std::string& key, const json& object)
	{
		auto camera = Json::parseUnsignedProperty(object, "camera", false);

		auto children = Json::parseUnsignedArrayProperty(object, "children", false);
		VERIFY(!children || children.value().size() > 0, "Gltf node '{}' empty property 'children'", key);

		auto skin = Json::parseUnsignedProperty(object, "skin", false);

		auto matrix = Json::parseFloatArrayProperty(object, "matrix", false);
		VERIFY(!matrix || matrix.value().size() == 16, "Gltf node '{}' property 'matrix' invalid size", key);

		auto mesh = Json::parseUnsignedProperty(object, "mesh", false);

		auto rotation = Json::parseFloatArrayProperty(object, "rotation", false);
		VERIFY(!rotation || rotation.value().size() == 4, "Gltf node '{}' property 'rotation' invalid size", key);

		auto scale = Json::parseFloatArrayProperty(object, "scale", false);
		VERIFY(!scale || scale.value().size() == 3, "Gltf node '{}' property 'scale' invalid size", key);

		auto translation = Json::parseFloatArrayProperty(object, "translation", false);
		VERIFY(!translation || translation.value().size() == 3, "Gltf node '{}' property 'translation' invalid size", key);

		auto weights = Json::parseFloatArrayProperty(object, "weights", false);
		VERIFY(!weights || weights.value().size() > 0, "Gltf node '{}' empty property 'weights'", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (camera)      node->camera = camera.value();
		if (children)    node->children = children.value();
		if (skin)        node->skin = skin.value();
		if (matrix)      std::copy(matrix.value().begin(), matrix.value().end(), node->matrix.begin());
		if (mesh)        node->mesh = mesh.value();
		if (rotation)    std::copy(rotation.value().begin(), rotation.value().end(), node->rotation.begin());
		if (scale)       std::copy(scale.value().begin(), scale.value().end(), node->scale.begin());
		if (translation) std::copy(translation.value().begin(), translation.value().end(), node->translation.begin());
		if (weights)     node->weights = weights.value();
		node->name = name ? name.value() : key;
	}

	void Gltf::parsePrimitive(glTF::Primitive* primitive, const std::string& key, const json& object)
	{
		auto attributes = Json::parseUnsignedObjectProperty(object, "attributes", true);
		VERIFY(attributes && attributes.value().size() > 0, "Gltf primitive '{}' empty property 'attributes'", key);

		auto indices = Json::parseUnsignedProperty(object, "indices", false);

		auto material = Json::parseUnsignedProperty(object, "material", false);

		auto mode = Json::parseUnsignedProperty(object, "mode", false);

		if (Json::hasProperty(object, "targets")) {

			auto targets = object["targets"];
			VERIFY(targets.is_array(), "Gltf primitive '{}' property 'targets' invalid type", key);

			for (auto& targetObject : targets) {

				std::map<std::string, uint32_t> target;

				for (auto& [key, propertyValue] : targetObject.items()) {
					auto value = Json::getPropertyValue<uint32_t>(propertyValue, json::value_t::number_unsigned);
					if (value) target.emplace(std::move(key), value.value());
				}

				VERIFY(target.size() > 0, "Gltf primitive '{}' empty 'target' object", key);
				primitive->targets.emplace_back(std::move(target));
			}
		}

		if (attributes) primitive->attributes = attributes.value();
		if (indices)    primitive->indices = indices.value();
		if (material)   primitive->material = material.value();
		if (mode)       primitive->mode = static_cast<unsigned char>(mode.value());
	}

	void Gltf::parseMesh(glTF::Mesh* mesh, const std::string& key, const json& object)
	{
		VERIFY(Json::hasProperty(object, "primitives"), "Gltf mesh '{}' missing required property 'primitives'", key);
		auto primitives = object["primitives"];
		VERIFY(primitives.is_array(), "Gltf mesh '{}' property 'primitives' invalid type", key);

		for (auto& primitiveObject : primitives) {
			glTF::Primitive primitive;
			parsePrimitive(&primitive, key, primitiveObject);
			mesh->primitives.emplace_back(std::move(primitive));
		}

		auto weights = Json::parseFloatArrayProperty(object, "weights", false);
		VERIFY(!weights || weights.value().size() > 0, "Gltf mesh '{}' empty property 'weights'", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (weights) mesh->weights = weights.value();
		mesh->name = name ? name.value() : key;
	}

	void Gltf::parseAccessor(glTF::Accessor* accessor, const std::string& key, const json& object)
	{
		auto bufferView = Json::parseUnsignedProperty(object, "bufferView", false);

		auto byteOffset = Json::parseUnsignedProperty(object, "byteOffset", false);

		auto componentType = Json::parseUnsignedProperty(object, "componentType", true);
		VERIFY(componentType, "Gltf accessor '{}' missing required property 'componentType'", key);

		auto normalized = Json::parseBoolProperty(object, "normalized", false);

		auto count = Json::parseUnsignedProperty(object, "count", true);
		VERIFY(count, "Gltf accessor '{}' missing required property 'count'", key);

		auto type = Json::parseStringProperty(object, "type", true);
		VERIFY(type, "Gltf accessor '{}' missing required property 'type'", key);

		auto max = Json::parseFloatArrayProperty(object, "max", false);
		VERIFY(!max || max.value().size() > 0, "Gltf accessor '{}' empty property 'max'", key);

		auto min = Json::parseFloatArrayProperty(object, "min", false);
		VERIFY(!min || min.value().size() > 0, "Gltf accessor '{}' empty property 'min'", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (bufferView) accessor->bufferView = bufferView.value();
		if (byteOffset) accessor->byteOffset = byteOffset.value();
		if (normalized) accessor->normalized = normalized.value();
		if (count)      accessor->count = count.value();
		if (type)       accessor->type = type.value();
		if (max)        accessor->max = max.value();
		if (min)        accessor->min = min.value();
		accessor->name = name ? name.value() : key;
	}

	void Gltf::parseBufferView(glTF::BufferView* bufferView, const std::string& key, const json& object)
	{
		auto buffer = Json::parseUnsignedProperty(object, "buffer", false);
		VERIFY(buffer, "Gltf bufferView '{}' missing required property 'buffer'", key);

		auto byteOffset = Json::parseUnsignedProperty(object, "byteOffset", false);

		auto byteLength = Json::parseUnsignedProperty(object, "byteLength", true);
		VERIFY(byteLength, "Gltf bufferView '{}' missing required property 'byteLength'", key);

		auto byteStride = Json::parseUnsignedProperty(object, "byteStride", false);

		auto target = Json::parseUnsignedProperty(object, "target", false);

		auto name = Json::parseStringProperty(object, "name", false);

		if (buffer)     bufferView->buffer = buffer.value();
		if (byteOffset) bufferView->byteOffset = byteOffset.value();
		if (byteLength) bufferView->byteLength = byteLength.value();
		if (byteStride) bufferView->byteStride = byteStride.value();
		if (target)     bufferView->target = target.value();
		bufferView->name = name ? name.value() : key;
	}

	void Gltf::parseBuffer(glTF::Buffer* buffer, const std::string& key, const json& object, std::map<uint32_t, std::shared_ptr<char[]>>* data)
	{
		auto uri = Json::parseStringProperty(object, "uri", false);

		// Load external binary data
		if (uri) {
			VERIFY(uri.value().find("data:", 0) == std::string::npos, "GltfFile embedded binary data is unsupported");
			data->emplace(std::stou(key), File::raw("assets/gltf/" + uri.value()));
		}

		auto byteLength = Json::parseUnsignedProperty(object, "byteLength", true);
		VERIFY(byteLength, "Gltf buffer '{}' missing required property 'byteLength'", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (uri)        buffer->uri = uri.value();
		if (byteLength) buffer->byteLength = byteLength.value();
		buffer->name = name ? name.value() : key;
	}

} // namespace Inferno
#endif
