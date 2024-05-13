/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/texture.h"
#include "ruc/format/log.h"

#include "inferno/asset/model.h"
#include "inferno/asset/texture.h"

namespace Inferno {

std::shared_ptr<Model> Model::create(std::string_view path)
{
	auto result = std::shared_ptr<Model>(new Model(path));

	Assimp::Importer importer; // importer destructor uses RAII cleanup
	const aiScene* scene = importer.ReadFile(
		path.data(),
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	VERIFY(scene != nullptr && (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0 && scene->mRootNode != nullptr,
	       "assimp loading file failed: {}", importer.GetErrorString());

	processScene(result, scene);
	processNode(result, scene->mRootNode, scene);

	return result;
}

void Model::processScene(std::shared_ptr<Model> model, const aiScene* scene)
{
	VERIFY(scene->HasMeshes(), "malformed model");
	VERIFY(scene->mNumTextures < 2, "unsupported model type");

	if (scene->mNumTextures == 1) {
		aiTexture* texture = scene->mTextures[0];
		model->m_texture = Texture2D::create(texture);
	}
}

void Model::processNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(model, mesh, scene);
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i) {
		processNode(model, node->mChildren[i], scene);
	}
}

void Model::processMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene)
{
	VERIFY(mesh->HasPositions(), "malformed model");
	VERIFY(mesh->HasNormals(), "malformed model");

	// Pre-allocate memory
	model->m_vertices = std::vector<Vertex>(mesh->mNumVertices);

	for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D v = mesh->mVertices[i];
		model->m_vertices[i].position = { v.x, v.y, v.z };
	}

	// Size of vertices == size of normals
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D normal = mesh->mNormals[i];
		model->m_vertices[i].normal = { normal.x, normal.y, normal.x };
	}

	if (mesh->HasTextureCoords(0)) {
		// Size of vertices == size of texture coordinates
		for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
			aiVector3D tc = mesh->mTextureCoords[0][i];
			model->m_vertices[i].textureCoordinates = { tc.x, tc.y };
		}
	}
	// TODO: position in the texture atlas

	if (mesh->HasFaces()) {
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j) {
				model->m_elements.push_back(face.mIndices[j]);
			}
		}
	}

	ruc::debug("mesh: {:p}", mesh->mTextureCoordsNames);
	// for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
	// 	ruc::debug("mesh: {}", mesh->mTextureCoordsNames[i]);
	// }

	ruc::debug("has texture: {}", scene->HasTextures());

	ruc::error("asset::model vert {}", model->m_vertices.size());
	ruc::error("asset::model elem {}", model->m_elements.size());
}

} // namespace Inferno
