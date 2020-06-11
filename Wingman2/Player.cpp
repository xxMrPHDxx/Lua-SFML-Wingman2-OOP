#include "pch.h"
#include "Player.h"

void Player::register_class(lua_State* L)
{
	// Create a new metatable of Player
	lua_newtable(L);

	// Set the __index
	lua_pushstring(L, "Player");
	lua_setfield(L, -2, "__index");

	// Set static method new to create the object
	lua_pushcfunction(L, create);
	lua_setfield(L, -2, "new");

	// Set as global
	lua_setglobal(L, "Player");
}

int Player::create(lua_State* L)
{
	// Get the 2 arguments if any
	float x = 0.f, y = 0.f;
	if (lua_gettop(L) == 3 && lua_isnumber(L, -2) && lua_isnumber(L, -1)) {
		x = lua_tonumber(L, -2);
		y = lua_tonumber(L, -1);
		lua_pop(L, 2);
	}

	// Create a new instance table
	lua_newtable(L);

	// Set the member methods
	luaL_Reg methods[] = {
		{ "destroy", destroy },
		{ "move", move },
		{ "update", update }
	};
	for (luaL_Reg& reg : methods) {
		lua_pushcfunction(L, reg.func);
		lua_setfield(L, -2, reg.name);
	}

	// Set the object pointer
	lua_pushlightuserdata(L, new Player());
	lua_setfield(L, -2, "ptr");

	// Set the metatable
	lua_getglobal(L, "Player");
	lua_setmetatable(L, -2);
	return 1;
}

int Player::destroy(lua_State* L)
{
	// Destroy the physics object, if any
	lua_getfield(L, -1, "physics");
	if (lua_istable(L, -1)) {
		PhysicsComponent* comp = PhysicsComponent::get_instance(L);
		delete comp;
	}
	lua_pop(L, 1);

	// Destroy the Player object
	Player* obj = get_instance(L);
	delete obj;
	return 1;
}

int Player::move(lua_State* L)
{
	// Check the arguments
	if (lua_gettop(L) != 3 || !lua_isinteger(L, -2) || !lua_isinteger(L, -1)) {
		std::cout << "[Error]:: Usage: Player:move(dir_x, dir_y)!" << std::endl;
		return 1;
	}

	// Get the arguments (dir_x, dir_y)
	sf::Vector2i dir(lua_tointeger(L, -2), lua_tointeger(L, -1));
	lua_pop(L, 2);

	// Get the Player instance
	Player& obj = *get_instance(L);

	// Get the global dt if any
	lua_getglobal(L, "dt");
	float dt = lua_isnumber(L, -1) ? float(lua_tonumber(L, -1)) : 0.0016f;
	lua_pop(L, 1);

	// Get the physics component, if any
	lua_getfield(L, -1, "physics");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	PhysicsComponent& comp = *PhysicsComponent::get_instance(L);
	lua_pop(L, 1);

	// Move the physics component
	comp.move(dt, dir.x, dir.y);
	return 1;
}

int Player::update(lua_State* L)
{
	// Get the player
	Player& obj = *get_instance(L);

	// Get the global dt if any
	lua_getglobal(L, "dt");
	float dt = lua_isnumber(L, -1) ? float(lua_tonumber(L, -1)) : 0.0016f;
	lua_pop(L, 1);

	// Get the physics component, if any
	lua_getfield(L, -1, "physics");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	PhysicsComponent* comp = PhysicsComponent::get_instance(L);
	lua_pop(L, 1);

	// Update the physics component
	comp->update(dt);
	return 1;
}

Player* Player::get_instance(lua_State* L)
{
	lua_getfield(L, -1, "ptr");
	Player* obj = (Player*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	return obj;
}
