#pragma once

#include "pch.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;
	sf::Clock global_clock;
public:
	Game(int width, int height, std::string title);
	~Game();

	static void register_class(lua_State*);

	static int create(lua_State*);
	static int destroy(lua_State*);
	
	static int set_framerate_limit(lua_State*);
	static int get_dt(lua_State*);
	static int get_key_code(lua_State*);
	static int get_mouse_position(lua_State*);
	static int is_running(lua_State*);
	static int is_event_closed(lua_State*);
	static int is_event_key_pressed(lua_State*);
	static int is_event_key_released(lua_State*);

	static int poll_event(lua_State*);
	static int close(lua_State*);
	static int clear(lua_State*);
	static int draw(lua_State*);
	static int display(lua_State*);
	static Game* get_game(lua_State*);
};

