texture = Resources.loadTexture("res/tile.png", "tex")
shader = Resources.loadShader("res/default.glsl")

material = Resources.makeMaterial("default", shader)

material.diffuse_tint = vec4.new(0.5, 1.0, 1.0, 1.0)
material.diffuse = texture

cube = Resources.loadMesh("primitive.cube")

entity = World.createEntity("Kyoob")

entity.renderer.mesh = cube
entity.renderer.material = material
entity.transform.position = vec3.new(-0.1, 0.0, 0.0)

function OnTick()
	entity.transform:rotate(0.0, 1.0, 2.0)
	entity.transform.position = vec3.new(0, math.cos(World.time()) * 0.6, 0.0)
end

