LuaScript = {

	camera = nil,

	initialize = function(self)
		--
	end,

	destroy = function(self)
		--
	end,

	update = function(self, deltaTime)
		self.camera = GetComponent("CameraComponent")
		LuaScript:updateOrthographic(deltaTime)
		LuaScript:updatePerspective(deltaTime)

		local tmp = self.camera.fov
		self.camera.fov = 30.0

		--
	end,

	updateOrthographic = function(self, deltaTime)
		--
	end,

	updatePerspective = function(self, deltaTime)
		--
	end,

}
