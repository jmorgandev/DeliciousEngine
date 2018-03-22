texture = Texture.Load("res/tile.png")
shader = Shader.Load("res/default.glsl")

material = Material.Make("default", shader)
material["diffuse_tint"] = {0.5, 1.0, 1.0, 1.0}
material["diffuse"] = texture

cube = Mesh.Load("primitive.cube")

entity = World.CreateEntity()
entity.renderer.mesh = cube
entity.renderer.material = material
entity.transform.position = {-1.0, 0.0, 0.0}

function Tick()
	entity.transform.Rotate(0.0, 1.0, 0.0)
	entity.transform.position = {0.0, Math.Cos(0.6, Time.time), 0.0}
end

texture = Resources.Load("res/tile.png")

material = Resources.New(AssetType.Material, "default")

Assets.Load("res/tile.png")
Resources.Load("res/tile.png")