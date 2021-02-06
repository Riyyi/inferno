#include "inferno/util/json.h"
#include "nlohmann/json.hpp"

#include "inferno/assert.h"
#include "inferno/io/gltffile.h"
#include "inferno/io/log.h"
#include "inferno/render/gltf.h"
#include <stdint.h>

namespace Inferno {

	Gltf::Gltf(const std::string& path)
		: m_path(path)
	{
		auto gltf = GlTFFile::read(path);
		ASSERT(gltf.first && gltf.second, "GlTF model was incomplete '{}'", path);

		json json = json::parse(gltf.first.get());

		// Properties

		// Asset
		// ---------------------------------

		{
			ASSERT(Json::hasProperty(json, "asset"), "GlTF model missing required property 'asset'");
			auto asset = json["asset"];

			auto copyright = Json::parseStringProperty(asset, "copyright", false);
			auto generator = Json::parseStringProperty(asset, "generator", false);
			auto version = Json::parseStringProperty(asset, "version", true);
			ASSERT(version, "GlTF model missing required property 'version'");
			ASSERT(version.value().compare("2.0") == 0, "GlTF version unsupported '{}'", version.value());
			auto minVersion = Json::parseStringProperty(asset, "minVersion", false);

			if (copyright)  m_model.asset.copyright = copyright.value();
			if (generator)  m_model.asset.generator = generator.value();
			if (version)    m_model.asset.version = version.value();
			if (minVersion) m_model.asset.minVersion = minVersion.value();
		}

		// Scene
		// ---------------------------------

		// Node
		// ---------------------------------

		// Mesh
		// ---------------------------------

		if (Json::hasProperty(json, "meshes")) {

			auto meshes = json["meshes"];
			ASSERT(meshes.is_array(), "GlTF model property 'meshes' invalid type");

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
			ASSERT(accessors.is_array(), "GlTF model property 'accessors' invalid type");

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
			ASSERT(bufferViews.is_array(), "GlTF model property 'bufferViews' invalid type");

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
			ASSERT(buffers.is_array(), "GlTF model property 'buffers' invalid type");

			for (auto& [key, object] : buffers.items()) {
				glTF::Buffer buffer;
				parseBuffer(&buffer, key, object);
				m_model.buffers.emplace_back(std::move(buffer));
			}
		}
	}

	Gltf::~Gltf()
	{
	}

	void Gltf::parsePrimitive(glTF::Primitive* primitive, const std::string& key, const json& object)
	{
		auto attributes = Json::parseUnsignedObjectProperty(object, "attributes", true);
		ASSERT(attributes && attributes.value().size() > 0, "GlTF primitive '{}' invalid property 'attributes'", key);
		auto indices = Json::parseUnsignedProperty(object, "indices", false);
		auto material = Json::parseUnsignedProperty(object, "material", false);
		auto mode = Json::parseUnsignedProperty(object, "mode", false);

		if (Json::hasProperty(object, "targets")) {

			auto targets = object["targets"];
			ASSERT(targets.is_array(), "GlTF primitive '{}' property 'targets' invalid type", key);

			for (auto& targetObject : targets) {

				std::map<std::string, uint32_t> target;

				for (auto& [key, propertyValue] : targetObject.items()) {
					auto value = Json::getPropertyValue<uint32_t>(propertyValue, json::value_t::number_unsigned);
					if (value) target.emplace(std::move(key), value.value());
				}

				ASSERT(target.size() > 0, "GlTF primitive '{}' empty 'target' object", key);
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
		ASSERT(Json::hasProperty(object, "primitives"), "GlTF mesh '{}' missing required property 'primitives'", key);
		auto primitives = object["primitives"];
		ASSERT(primitives.is_array(), "GlTF mesh '{}' property 'primitives' invalid type", key);
		for (auto& primitiveObject : primitives) {
			glTF::Primitive primitive;
			parsePrimitive(&primitive, key, primitiveObject);
			mesh->primitives.emplace_back(std::move(primitive));
		}

		auto weights = Json::parseDoubleArrayProperty(object, "weights", false);
		ASSERT(!weights || weights.value().size() > 0, "GlTF mesh '{}' empty 'weights' property", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (weights) mesh->weights = weights.value();
		mesh->name = name ? name.value() : key;
	}

	void Gltf::parseAccessor(glTF::Accessor* accessor, const std::string& key, const json& object)
	{
		auto bufferView = Json::parseUnsignedProperty(object, "bufferView", false);

		auto byteOffset = Json::parseUnsignedProperty(object, "byteOffset", false);

		auto componentType = Json::parseUnsignedProperty(object, "componentType", true);
		ASSERT(componentType, "GlTF accessor '{}' missing required property 'componentType'", key);

		auto normalized = Json::parseBoolProperty(object, "normalized", false);

		auto count = Json::parseUnsignedProperty(object, "count", true);
		ASSERT(count, "GlTF accessor '{}' missing required property 'count'", key);

		auto type = Json::parseStringProperty(object, "type", true);
		ASSERT(type, "GlTF accessor '{}' missing required property 'type'", key);

		auto max = Json::parseDoubleArrayProperty(object, "max", false);
		ASSERT(!max || max.value().size() > 0, "GlTF accessor '{}' empty 'max' property", key);

		auto min = Json::parseDoubleArrayProperty(object, "min", false);
		ASSERT(!min || min.value().size() > 0, "GlTF accessor '{}' empty 'min' property", key);

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
		ASSERT(buffer, "GlTF bufferView '{}' missing required property 'buffer'", key);

		auto byteOffset = Json::parseUnsignedProperty(object, "byteOffset", false);

		auto byteLength = Json::parseUnsignedProperty(object, "byteLength", true);
		ASSERT(byteLength, "GlTF bufferView '{}' missing required property 'byteLength'", key);

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

	void Gltf::parseBuffer(glTF::Buffer* buffer, const std::string& key, const json& object)
	{
		auto uri = Json::parseStringProperty(object, "buffer", false);

		auto byteLength = Json::parseUnsignedProperty(object, "byteLength", true);
		ASSERT(byteLength, "GlTF buffer '{}' missing required property 'byteLength'", key);

		auto name = Json::parseStringProperty(object, "name", false);

		if (uri)        buffer->uri = uri.value();
		if (byteLength) buffer->byteLength = byteLength.value();
		buffer->name = name ? name.value() : key;
	}

} // namespace Inferno
