#include "pch.h"
#include "Game.h"

Game::Game(int w, int h, std::string title) {
	window = new sf::RenderWindow(sf::VideoMode(w, h), title, sf::Style::Titlebar | sf::Style::Close);
}

Game::~Game() {
	delete window;
}

void Game::register_class(lua_State* L)
{
	// Create empty table
	lua_newtable(L);

	// Create __index property
	lua_pushstring(L, "Game");
	lua_setfield(L, -2, "__index");

	// Push static methods Game:new()
	lua_pushcfunction(L, create);
	lua_setfield(L, -2, "new");

	// Set to global variable
	lua_setglobal(L, "Game");
}

int Game::create(lua_State* L)
{
	// Get the 3 arguments if any
	int w = 640, h = 480;
	std::string title = "Untitled Game";
	if (lua_gettop(L) == 4 && lua_isinteger(L, -3) && lua_isinteger(L, -2) && lua_isstring(L, -1)) {
		w = lua_tointeger(L, -3);
		h = lua_tointeger(L, -2);
		title = lua_tostring(L, -1);
		lua_pop(L, 3);
	}

	// Create new table for our Game object
	lua_newtable(L);

	// Register member methods
	luaL_Reg methods[] = {
		{ "destroy", destroy },
		{ "set_framerate_limit", set_framerate_limit },
		{ "get_dt", get_dt },
		{ "get_key_code", get_key_code },
		{ "get_mouse_position", get_mouse_position },
		{ "is_running", is_running },
		{ "poll_event", poll_event },
		{ "is_event_closed", is_event_closed },
		{ "is_event_key_pressed", is_event_key_pressed },
		{ "is_event_key_released", is_event_key_released },
		{ "close", close },
		{ "clear", clear },
		{ "draw", draw },
		{ "display", display }
	};
	for (luaL_Reg& reg : methods) {
		lua_pushcfunction(L, reg.func);
		lua_setfield(L, -2, reg.name);
	}

	// Set game object
	lua_pushlightuserdata(L, new Game(w, h, title));
	lua_setfield(L, -2, "ptr");

	// Set the object metatable to Game table
	lua_getglobal(L, "Game");
	lua_setmetatable(L, -2);
	return 1;
}

int Game::destroy(lua_State* L) {
	Game* game = get_game(L);
	delete game;
	return 1;
}

int Game::set_framerate_limit(lua_State* L) {
	// Check the argument
	if (lua_gettop(L) != 2 || !lua_isinteger(L, -1)) {
		std::cout << "[Error]:: Usage: Game:set_framerate_limit(integer)!" << std::endl;
		return 1;
	}

	// Get the argument
	int limit = lua_tointeger(L, -1);
	lua_pop(L, 1);

	// Get the game object and set the limit
	Game& game = *get_game(L);
	game.window->setFramerateLimit(limit);
	return 1;
}

int Game::get_dt(lua_State * L) {
	Game& game = *get_game(L);
	
	lua_pushnumber(L, game.global_clock.restart().asMilliseconds() / 1000.f);
	return 1;
}

int Game::get_key_code(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushinteger(L, game.event.key.code);
	return 1;
}

int Game::get_mouse_position(lua_State* L) {
	Game& game = *get_game(L);

	// Mouse position
	sf::Vector2f mouse = game.window->mapPixelToCoords(sf::Mouse::getPosition(*game.window));

	// Create empty table
	lua_newtable(L);
	lua_pushinteger(L, mouse.x);
	lua_seti(L, -2, 1);
	lua_pushinteger(L, mouse.y);
	lua_seti(L, -2, 2);
	return 1;
}

int Game::is_running(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushboolean(L, game.window->isOpen());
	return 1;
}

int Game::is_event_closed(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushboolean(L, game.event.type == sf::Event::Closed);
	return 1;
}

int Game::is_event_key_pressed(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushboolean(L, game.event.type == sf::Event::KeyPressed);
	return 1;
}

int Game::is_event_key_released(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushboolean(L, game.event.type == sf::Event::KeyReleased);
	return 1;
}

int Game::poll_event(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushboolean(L, game.window->pollEvent(game.event));
	return 1;
}

int Game::close(lua_State* L) {
	Game& game = *get_game(L);

	game.window->close();
	return 1;
}

int Game::clear(lua_State* L) {
	// Check for arguments
	bool has_arg = false;
	int r, g, b, a;
	if (lua_gettop(L) >= 4 && lua_gettop(L) <= 5) {
		for (int i = 1; i <= lua_gettop(L) - 1; i++) {
			if (!lua_isinteger(L, -i)) break;
			has_arg = true;
		}
	}

	// Get the corresponding arguments
	if (has_arg) {
		int off = lua_gettop(L) == 4 ? -3 : -4;
		int* ptr[] = { &r, &g, &b };
		for (int i = 0; i < 3; i++) {
			*ptr[i] = (int)lua_tointeger(L, off + i);
		}
		a = lua_gettop(L) == 4 ? 255 : lua_tointeger(L, -1);
		lua_pop(L, lua_gettop(L) == 4 ? 3 : 4);
	}

	Game& game = *get_game(L);

	if (has_arg) game.window->clear(sf::Color(r, g, b, a));
	else game.window->clear();
	return 1;
}

int Game::draw(lua_State* L)
{
	// Check the argument
	if (lua_gettop(L) != 2 || !lua_isuserdata(L, -1)) {
		std::cout << "[Error] Argument expects a sprite! Got " << lua_typename(L, -1) << "." << std::endl;
		return 1;
	}

	// Get the argument
	sf::Sprite& spr = *((sf::Sprite*)lua_touserdata(L, -1));
	lua_pop(L, 1);

	// Get the game object
	Game& game = *get_game(L);

	// Draw the sprite
	game.window->draw(spr);
	return 1;
}

int Game::display(lua_State* L) {
	Game& game = *get_game(L);

	game.window->display();
	return 1;
}

Game* Game::get_game(lua_State* L) {
	lua_getfield(L, -1, "ptr");
	Game* game = (Game*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	return game;
}