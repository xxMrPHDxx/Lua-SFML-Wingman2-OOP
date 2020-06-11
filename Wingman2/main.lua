game = Game:new(640, 480, "Wingman 2")
game:set_framerate_limit(60)

player = Player:new()
player.physics = PhysicsComponent:new(150, 150, 200, 200)

keys = {}

while game:is_running() do
	while game:poll_event() do
		if game:is_event_closed() then game:close() end
		if game:is_event_key_pressed() then
			keys[game:get_key_code()] = true
		end
		if game:is_event_key_released() then
			keys[game:get_key_code()] = false
		end
	end

	dt = game:get_dt()
	-- print("Delta time: " .. dt)

	-- Input handling
	if keys[ 0] then player:move(-1,  0) print("Moving left ") end
	if keys[ 3] then player:move( 1,  0) end
	if keys[18] then player:move( 0, -1) end
	if keys[14] then player:move( 0,  1) end

	-- Update
	player:update()
	print(table.unpack(player.physics:get_speed()))

	-- Rendering

	game:display()
end

game:destroy()
player:destroy()