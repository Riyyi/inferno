TRANSLATE_SPEED = 2.5
ROTATE_SPEED = 90.0
ZOOM_SENSITIVITY = 2.5
MOUSE_SENSITIVITY = 0.25

LuaScript = {

	camera = nil,

	initialize = function(self)
		--
	end,

	destroy = function(self)
		--
	end,

	update = function(self, deltaTime)

		self.camera = getCameraComponent()

		if self.camera.type == CameraType.Orthographic then
			LuaScript:updateOrthographic(deltaTime)
		elseif self.camera.type == CameraType.Perspective then
			LuaScript:updatePerspective(deltaTime)
		end

	end,

	updateOrthographic = function(self, deltaTime)

		-- Update camera rotation

		cameraRotateSpeed = ROTATE_SPEED * deltaTime

		if Input.isKeyPressed(keyCode("GLFW_KEY_Q")) then
			self.transform.rotate.z = self.transform.rotate.z - cameraRotateSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_E")) then
			self.transform.rotate.z = self.transform.rotate.z + cameraRotateSpeed
		end

		if self.transform.rotate.z > 180.0 then
			self.transform.rotate.z = self.transform.rotate.z - 360.0
		elseif self.transform.rotate.z <= -180.0 then
			self.transform.rotate.z = self.transform.rotate.z + 360.0
		end

		-- Update camera translation

		cameraTranslateSpeed = TRANSLATE_SPEED * deltaTime

		-- WASD movement
		if Input.isKeyPressed(keyCode("GLFW_KEY_W")) then
			self.transform.translate.x = self.transform.translate.x + -math.sin(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
			self.transform.translate.y = self.transform.translate.y +  math.cos(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_S")) then
			self.transform.translate.x = self.transform.translate.x - -math.sin(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
			self.transform.translate.y = self.transform.translate.y -  math.cos(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_A")) then
			self.transform.translate.x = self.transform.translate.x -  math.cos(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
			self.transform.translate.y = self.transform.translate.y -  math.sin(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_D")) then
			self.transform.translate.x = self.transform.translate.x +  math.cos(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
			self.transform.translate.y = self.transform.translate.y +  math.sin(glm.radians(self.transform.rotate.z)) * cameraTranslateSpeed;
		end

		-- Update camera zoom

		zoomSpeed = ZOOM_SENSITIVITY * deltaTime;

		if Input.isKeyPressed(keyCode("GLFW_KEY_EQUAL")) then
			self.camera.zoomLevel = self.camera.zoomLevel - zoomSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_MINUS")) then
			self.camera.zoomLevel = self.camera.zoomLevel + zoomSpeed
		end
		self.camera.zoomLevel = math.max(self.camera.zoomLevel, 0.25);
		self.camera.zoomLevel = math.min(self.camera.zoomLevel, 10.0);

	end,

	updatePerspective = function(self, deltaTime)

		-- Get mouse movement offset compared to last frame
		xOffset = Input.getXOffset() * MOUSE_SENSITIVITY;
		yOffset = Input.getYOffset() * MOUSE_SENSITIVITY;
		self.camera.yaw = self.camera.yaw + xOffset;
		self.camera.pitch = self.camera.pitch + yOffset;

		-- Prevent gimbal lock
		if self.camera.pitch > 89.0 then self.camera.pitch = 89.0 end
		if self.camera.pitch < -89.0 then self.camera.pitch = -89.0 end

		-- Update camera rotation, by calculating direction vector via yaw and pitch

		self.transform.rotate = glm.vec3(
			math.cos(glm.radians(self.camera.pitch)) * math.cos(glm.radians(self.camera.yaw)),
			math.sin(glm.radians(self.camera.pitch)),
			math.cos(glm.radians(self.camera.pitch)) * math.sin(glm.radians(self.camera.yaw))
		)
		self.transform.rotate = glm.normalize(self.transform.rotate)
		-- The direction vector is based on
		-- Camera direction (z): normalize(position - target)
		-- Right axis       (x): normalize(cross(up, direction))
		-- Up axis          (y): cross(direction, right)

		-- Source: https://learnopengl.com/img/getting-started/camera_axes.png

		-- Cross = combination of two vectors in 3D space,
		--         where result is always perpendicular to both of the vectors

		-- Update camera translation

		cameraSpeed = TRANSLATE_SPEED * deltaTime

		-- WASD movement
		if Input.isKeyPressed(keyCode("GLFW_KEY_W")) then
			self.transform.translate = self.transform.translate + self.transform.rotate * cameraSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_S")) then
			self.transform.translate = self.transform.translate - self.transform.rotate * cameraSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_A")) then
			self.transform.translate = self.transform.translate - glm.normalize(glm.cross(self.transform.rotate, self.camera.up)) * cameraSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_D")) then
			self.transform.translate = self.transform.translate + glm.normalize(glm.cross(self.transform.rotate, self.camera.up)) * cameraSpeed
		end
		-- Up / down movement
		if Input.isKeyPressed(keyCode("GLFW_KEY_SPACE")) then
			self.transform.translate.y = self.transform.translate.y + cameraSpeed
		end
		if Input.isKeyPressed(keyCode("GLFW_KEY_LEFT_SHIFT")) then
			self.transform.translate.y = self.transform.translate.y - cameraSpeed
		end

	end,

}
