/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace Inferno {

class Renderer3D {
public:
	Renderer3D();
	virtual ~Renderer3D();
};

} // namespace Inferno

// - what data do you need?
// - gltf
// - how are we going to batch
// - mesh (gltf) -> material -> shader rendering
