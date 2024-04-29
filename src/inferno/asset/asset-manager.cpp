/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <memory> // std::shared_ptr, std::static_pointer_cast
#include <string>
#include <string_view>

#include "ruc/format/log.h"

#include "inferno/asset/asset-manager.h"
#include "inferno/asset/shader.h"

namespace Inferno {

// -----------------------------------------

AssetManager::AssetManager(s)
{
	ruc::info("AssetManager initialized");
}

AssetManager::~AssetManager()
{
}

void AssetManager::add(std::string_view path, std::shared_ptr<Asset> asset)
{
	// Construct (key, value) pair and insert it into the unordered_map
	auto stringPath = std::string(path.begin(), path.end());
	m_assetList.emplace(std::move(stringPath), std::move(asset));
}

bool AssetManager::exists(std::string_view path)
{
	return m_assetList.find(path.data()) != m_assetList.end();
}

void AssetManager::remove(std::string_view path)
{
	if (exists(path)) {
		m_assetList.erase(path.data());
	}
}

void AssetManager::remove(std::shared_ptr<Asset> asset)
{
	if (exists(asset->path())) {
		m_assetList.erase(asset->path());
	}
}

} // namespace Inferno
