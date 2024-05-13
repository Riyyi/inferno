/*
 * Copyright (C) 2024 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory> // std::shared_ptr
#include <string>
#include <string_view>
#include <unordered_map>

#include "ruc/meta/assert.h"
#include "ruc/meta/types.h"
#include "ruc/singleton.h"

namespace Inferno {

class Asset {
public:
	virtual ~Asset() = default;

	std::string path() const { return m_path; }

	// -------------------------------------

	std::string className() const { return typeid(*this).name(); }

	template<typename T>
	bool fastIs() const = delete;

	virtual bool isFont() const { return false; }
	virtual bool isModel() const { return false; }
	virtual bool isShader() const { return false; }
	virtual bool isTexture() const { return false; }
	virtual bool isTexture2D() const { return false; }
	virtual bool isTextureCubemap() const { return false; }

protected:
	Asset(std::string_view path)
		: m_path(std::string(path.begin(), path.end()))
	{
	}

protected:
	std::string m_path;
};

// -----------------------------------------

template<typename T>
concept IsAsset = std::same_as<Asset, T> || std::derived_from<T, Asset>;

// -----------------------------------------

class AssetManager : public ruc::Singleton<AssetManager> {
public:
	AssetManager(s);
	virtual ~AssetManager();

	void add(std::string_view path, std::shared_ptr<Asset> asset);
	bool exists(std::string_view path);
	void remove(std::string_view path);
	void remove(std::shared_ptr<Asset> asset);

	template<IsAsset T>
	std::shared_ptr<T> get(std::string_view path)
	{
		if (!exists(path)) {
			return nullptr;
		}

		auto asset = m_assetList.at(path.data());
		VERIFY(is<T>(asset.get()), "expected asset {}, got {}", typeid(T).name(), asset->className());
		return std::static_pointer_cast<T>(asset);
	}

	template<IsAsset T>
	std::shared_ptr<T> load(std::string_view path)
	{
		if (exists(path)) {
			return get<T>(path);
		}

		auto asset = T::create(path);
		add(path, asset);
		return asset;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<Asset>> m_assetList;
};

} // namespace Inferno
