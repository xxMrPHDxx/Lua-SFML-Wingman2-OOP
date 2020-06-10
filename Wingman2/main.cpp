#include "pch.h"

#include "Game.h"

int main() {
	lua_State* L = luaL_newstate();

	// Load all libraries
	luaL_openlibs(L);

	Game::register_class(L);

	if (luaL_dofile(L, "main.lua")) {
		std::cout << "Error: " << lua_tostring(L, -1) << std::endl;
	}

	return 0;
}