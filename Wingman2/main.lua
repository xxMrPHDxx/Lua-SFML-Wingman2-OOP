require "utils"

game = Game:new(640, 480, "Wingman 2")
game:set_framerate_limit(60)

player = Player:new()
player:set_sprite_scale(0.8)
player.physics = PhysicsComponent:new(150, 150, 200, 200)
player.draw = function(game)
	game:draw(player:get_auras_sprite())
	game:draw(player:get_lwing_sprite())
	game:draw(player:get_rwing_sprite())
	game:draw(player:get_cpits_sprite())
end

-- Texture loading...
for _, path in pairs(lines_from_file("Textures/Accessories/Auras/list.txt")      ) do
	if not player:load_auras_texture(path) then print("Failed to load '" .. path .. "'!") end
end
for _, path in pairs(lines_from_file("Textures/Accessories/Cockpits/list.txt")      ) do
	if not player:load_cpits_texture(path) then print("Failed to load '" .. path .. "'!") end
end
for _, path in pairs(lines_from_file("Textures/Accessories/Left_Wings/list.txt")      ) do
	if not player:load_lwing_texture(path) then print("Failed to load '" .. path .. "'!") end
end
for _, path in pairs(lines_from_file("Textures/Accessories/Right_Wings/list.txt")      ) do
	if not player:load_rwing_texture(path) then print("Failed to load '" .. path .. "'!") end
end

PI = 3.1415
rad_to_deg = 180.0 / PI

time = 0.0
keys = {}

while game:is_running() do
	while game:poll_event() do
		if game:is_event_closed() then game:close() end
		if game:is_event_key_pressed() then
			key_code = game:get_key_code()
			-- print("Key " .. key_code)
			keys[key_code] = true
			if key_code == 20 then player:cycle_auras(1) end
			if key_code ==  8 then player:cycle_cpits(1) end
			if key_code == 14 then player:cycle_lwing(1) end
			if key_code == 15 then player:cycle_rwing(1) end
		end
		if game:is_event_key_released() then
			keys[game:get_key_code()] = false
		end
	end

	dt = game:get_dt()
	time = time + dt

	local mouse_x, mouse_y = table.unpack(game:get_mouse_position())

	-- Input handling
	if keys[ 0] then player:move(-1,  0) end
	if keys[ 3] then player:move( 1,  0) end
	if keys[22] then player:move( 0, -1) end
	if keys[18] then player:move( 0,  1) end

	-- Update
	player:update(mouse_x, mouse_y)

	-- Rendering
	game:clear(20, 20, 20, 20)

	---[[
	game:draw(player:get_auras_sprite())
	game:draw(player:get_lwing_sprite())
	game:draw(player:get_rwing_sprite())
	game:draw(player:get_cpits_sprite())
	--]]
	-- player.draw(game)

	game:display()
end

game:destroy()
player:destroy()