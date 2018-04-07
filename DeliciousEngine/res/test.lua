local CubeTest = {}

function CubeTest:load()
	
	--[[setmetatable(self, {
		__index = self.entity,
		__newindex = function(t, k, v)
			if t.entity[k] then
				t.entity.k = v
			else
				rawset(t, k, v)
			end
		end
		}
	)]]
	print(self)
	print(self.entity)

	texture = Resources.loadTexture("res/tile.png", "tex")
	print("Texture is " .. str(texture))
	shader = Resources.loadShader("res/default.glsl")
	print("Shader is " .. str(shader))

	material = Resources.makeMaterial("default", shader)
	print("Material is " .. str(material))

	material.diffuse = texture
	print("Material.diffuse_tint is " .. str(material.diffuse_tint))
	print("Material.diffuse is " .. str(material.diffuse))

	cube = Resources.loadMesh("primitive.cube")
	print("Cube is " .. str(cube))

	self.renderer.mesh = cube
	print("Renderer.mesh is " .. str(self.renderer.mesh))
	self.renderer.material = material
	print("Renderer.material is " .. str(self.renderer.material))
	self.transform.position = vec3.new(0.0, 0.0, 0.0)

	self.thing = 120
	print(self.thing)
	print(self.entity.thing)
end

function CubeTest:update()
	--self.transform:rotate(0.0, 1.0, 2.0)
	--if self.thing then print(self.thing) end
end

return CubeTest