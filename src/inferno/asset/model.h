/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <memory>
#include <span>
#include <vector>

#include "assimp/scene.h"

#include "inferno/asset/asset-manager.h"
#include "inferno/render/renderer.h"

namespace Inferno {

class Texture2D;

class Model final : public Asset {
public:
	virtual ~Model() {}

	// Factory function
	static std::shared_ptr<Model> create(std::string_view path);

	std::span<const Vertex> vertices() const { return m_vertices; }
	std::span<const uint32_t> elements() const { return m_elements; }
	std::shared_ptr<Texture2D> texture() const { return m_texture; }

private:
	Model(std::string_view path)
		: Asset(path)
	{
	}

	static void processScene(std::shared_ptr<Model> model, const aiScene* scene);
	static void processNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene);
	static void processMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiMatrix4x4 parentTransform = aiMatrix4x4());

	virtual bool isModel() const override { return true; }

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_elements;
	// Some file formats embed their texture
	std::shared_ptr<Texture2D> m_texture;
};

// clang-format off
template<>
inline bool Asset::fastIs<Model>() const { return isModel(); }
// clang-format on

} // namespace Inferno

// TODO: figure out this weird thing: https://github.com/assimp/assimp/blob/master/BUILDBINARIES_EXAMPLE.bat#L6-L10
