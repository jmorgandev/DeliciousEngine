local enemy = {}

local function enemy:init()
	mesh = Resources.Load("primitive.cube")
	
	material.SetTexture("diffuse", mesh)
end

return enemy