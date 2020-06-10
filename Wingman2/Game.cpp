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
		std::cout << "Got 3 arguments for Game:new!" << std::endl;
		w = lua_tointeger(L, -3);
		h = lua_tointeger(L, -2);
		title = lua_tostring(L, -1);
		lua_pop(L, 3);
	}

	std::cout << "Top: " << lua_gettop(L) << std::endl;
	for (int i = 1; i <= lua_gettop(L); i++) {
		std::cout << "Stack " << i << ": " << lua_typename(L, i) << std::endl;
	}

	// Create new table for our Game object
	lua_newtable(L);

	// Register member methods
	luaL_Reg methods[] = {
		{ "destroy", destroy },
		{ "get_key_code", get_key_code },
		{ "is_running", is_running },
		{ "poll_event", poll_event },
		{ "is_event_closed", is_event_closed },
		{ "is_event_key_pressed", is_event_key_pressed },
		{ "is_event_key_released", is_event_key_released },
		{ "close", close },
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
	lua_getfield(L, -1, "ptr");
	Game* game = (Game*) lua_touserdata(L, -1);
	lua_pop(L, 1);
	delete game;
	return 1;
}

int Game::get_key_code(lua_State* L) {
	Game& game = *get_game(L);

	lua_pushinteger(L, game.event.key.code);
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