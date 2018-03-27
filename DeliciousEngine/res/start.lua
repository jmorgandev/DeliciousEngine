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

function Something(speed, magnitude, offset)
	return ((math.cos((World.time() + offset) * speed) + 1.0) / 2.0) * magnitude
end

function OnTick()
	entity.transform:rotate(0.0, 1.0, 2.0)
	entity.transform.position = vec3.new(0, Something(1, 1, 0) - 0.5, 0.0)
	material.test = vec4.new(Something(2, 1, 0), Something(2.2, 1, 0.3), Something(2.3, 1, 0.5), 1.0)
end

