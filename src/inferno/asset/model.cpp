/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint32_t
#include <memory>  // std::shared_ptr

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/texture.h"

#include "inferno/asset/model.h"
#include "inferno/asset/texture.h"

namespace Inferno {

std::shared_ptr<Model> Model::create(std::string_view path)
{
	auto result = std::shared_ptr<Model>(new Model(path));

	Assimp::Importer importer; // importer destructor uses RAII cleanup
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
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
	VERIFY(scene->mNumTextures < 2, "unsupported model type: {}/1", scene->mNumTextures);

	if (scene->mNumTextures == 1) {
		aiTexture* texture = scene->mTextures[0];
		model->m_texture = Texture2D::create(texture);
	}
}

void Model::processNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(model, mesh, scene, node->mTransformation);
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i) {
		processNode(model, node->mChildren[i], scene);
	}
}

void Model::processMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiMatrix4x4 parentTransform)
{
	VERIFY(mesh->HasPositions(), "malformed model");
	VERIFY(mesh->HasNormals(), "malformed model");

	// Pre-allocate memory
	size_t startIndex = model->m_vertices.size();
	model->m_vertices.resize(startIndex + mesh->mNumVertices);

	for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D v = parentTransform * mesh->mVertices[i];
		model->m_vertices[startIndex + i].position = { v.x, v.y, v.z };
	}

	// Size of vertices == size of normals
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D normal = mesh->mNormals[startIndex + i];
		model->m_vertices[startIndex + i].normal = { normal.x, normal.y, normal.z };
	}

	if (mesh->HasTextureCoords(0)) {
		// Size of vertices == size of texture coordinates
		for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
			aiVector3D tc = mesh->mTextureCoords[0][i];
			model->m_vertices[startIndex + i].textureCoordinates = { tc.x, tc.y };
		}
	}
	// TODO: position in the texture atlas

	if (mesh->HasFaces()) {
		// Pre-allocate memory
		size_t startIndex2 = model->m_elements.size();
		model->m_elements.resize(startIndex2 + (mesh->mNumFaces * 3)); // assuming triangles

		size_t offset = 0;
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			VERIFY(face.mNumIndices == 3, "unsupported face type: {}", face.mNumIndices);
			for (uint32_t j = 0; j < face.mNumIndices; ++j, ++offset) {
				// Indices are referenced relative to vertices[0], if there are multiple meshes,
				// then the indices need to be offset by the total amount of vertices
				model->m_elements[startIndex2 + (i * 3) + j] = startIndex + face.mIndices[j];
			}
		}
	}
}

} // namespace Inferno
