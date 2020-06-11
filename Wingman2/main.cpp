#include "pch.h"

#include "Game.h"
#include "PhysicsComponent.h"
#include "Player.h"

int main() {
	lua_State* L = luaL_newstate();

	// Load all libraries
	luaL_openlibs(L);

	Game::register_class(L);
	PhysicsComponent::register_class(L);
	Player::register_class(L);

	if (luaL_dofile(L, "main.lua")) {
		std::cout << "Error: " << lua_tostring(L, -1) << std::endl;
	}

	return 0;
}