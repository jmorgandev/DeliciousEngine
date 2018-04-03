local CubeTest = {}

function CubeTest:load()
	print(self.entity)
	print(self.name)

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
end

function CubeTest:update()
	self.transform:rotate(0.0, 1.0, 2.0)
end

return CubeTest