game = Game:new(640, 480, "Wingman 2")

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

	game:display()
end

game:destroy()