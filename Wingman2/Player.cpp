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
		{ "set_angle", set_angle },
		{ "set_sprite_scale", set_sprite_scale },
		{ "get_auras_sprite", get_auras_sprite },
		{ "get_cpits_sprite", get_cpits_sprite },
		{ "get_lwing_sprite", get_lwing_sprite },
		{ "get_rwing_sprite", get_rwing_sprite },
		{ "load_auras_texture", load_auras_texture },
		{ "load_cpits_texture", load_cpits_texture },
		{ "load_lwing_texture", load_lwing_texture },
		{ "load_rwing_texture", load_rwing_texture },
		{ "cycle_auras", cycle_auras },
		{ "cycle_cpits", cycle_cpits },
		{ "cycle_lwing", cycle_lwing },
		{ "cycle_rwing", cycle_rwing },
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

int Player::set_angle(lua_State* L)
{
	// Check the argument
	if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) {
		return 1;
	}

	// Get the argument (angle in degrees)
	float angle = lua_tonumber(L, -1);
	lua_pop(L, 1);

	// Get the player object
	Player& obj = *get_instance(L);

	// Set the angle
	obj.angle = angle;
	return 1;
}

int Player::set_sprite_scale(lua_State* L)
{
	// Check the argument
	if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) {
		return 1;
	}

	// Get the argument (scale)
	float scale = lua_tonumber(L, -1);
	lua_pop(L, 1);

	// Get the player
	Player& obj = *get_instance(L);

	// Set the sprite scale for all available sprites
	for (sf::Sprite* spr : { &obj.spr_auras, &obj.spr_cpits, &obj.spr_lwing, &obj.spr_rwing }) {
		spr->setScale(scale, scale);
	}
	return 1;
}

int Player::get_auras_sprite(lua_State* L)
{
	get_sprite(L, 0);
	return 1;
}

int Player::get_cpits_sprite(lua_State* L)
{
	get_sprite(L, 1);
	return 1;
}

int Player::get_lwing_sprite(lua_State* L)
{
	get_sprite(L, 2);
	return 1;
}

int Player::get_rwing_sprite(lua_State* L)
{
	get_sprite(L, 3);
	return 1;
}

int Player::load_auras_texture(lua_State* L)
{
	lua_pushboolean(L, load_texture(L, 0));
	return 1;
}

int Player::load_cpits_texture(lua_State* L)
{
	lua_pushboolean(L, load_texture(L, 1));
	return 1;
}

int Player::load_lwing_texture(lua_State* L)
{
	lua_pushboolean(L, load_texture(L, 2));
	return 1;
}

int Player::load_rwing_texture(lua_State* L)
{
	lua_pushboolean(L, load_texture(L, 3));
	return 1;
}

int Player::cycle_auras(lua_State* L)
{
	cycle_texture(L, 0);
	return 1;
}

int Player::cycle_cpits(lua_State* L)
{
	cycle_texture(L, 1);
	return 1;
}

int Player::cycle_lwing(lua_State* L)
{
	cycle_texture(L, 2);
	return 1;
}

int Player::cycle_rwing(lua_State* L)
{
	cycle_texture(L, 3);
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
	float dt = lua_isnumber(L, -1) ? float(lua_tonumber(L, -1)) : 0.016f;
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
	// Get the arguments (mouse_x, mouse_y)
	sf::Vector2f mouse;
	if (lua_gettop(L) == 3 && lua_isnumber(L, -2) && lua_isnumber(L, -1)) {
		mouse = sf::Vector2f(lua_tonumber(L, -2), lua_tonumber(L, -1));
		lua_pop(L, 2);
	}

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
	PhysicsComponent& comp = *PhysicsComponent::get_instance(L);
	lua_pop(L, 1);

	// Get the position
	sf::Vector2f pos = comp.get_position();

	// Update the physics component
	obj.angle = std::atan2(mouse.y - pos.y, mouse.x - pos.x) + PI / 2.f;
	comp.update(dt);

	// Check if all part textures have at least a texture to be used
	if (!obj.tex_auras.size() || !obj.tex_cpits.size() || !obj.tex_lwing.size() || !obj.tex_rwing.size())
		return 1;

	// Set the corresponding texture
	obj.spr_auras.setTexture(obj.tex_auras[obj.idx_auras]);
	obj.spr_cpits.setTexture(obj.tex_cpits[obj.idx_cpits]);
	obj.spr_lwing.setTexture(obj.tex_lwing[obj.idx_lwing]);
	obj.spr_rwing.setTexture(obj.tex_rwing[obj.idx_rwing]);

	// Get the origin
	sf::Vector2f origin(comp.get_width() / 2, comp.get_height() / 2);

	// Get vector 1 (mouse to origin)
	sf::Vector2f v1 = mouse - origin;

	// Get vector 2 (speed)
	sf::Vector2f v2 = comp.get_speed();

	// Calculate speed
	float speed = magnitude(v2);

	// Get the time
	lua_getglobal(L, "time");
	float time = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 0.f;
	lua_pop(L, 1);

	// Calculate the scale
	float scale = 20.f * speed / comp.get_max_speed();
	sf::Vector2f heading = normalize(v2);

	for (sf::Sprite* spr : { &obj.spr_auras, &obj.spr_cpits, &obj.spr_lwing, &obj.spr_rwing }) {
		// Set the position
		if (spr == &obj.spr_lwing || spr == &obj.spr_rwing) {
			float angle = spr == &obj.spr_lwing ? -170.f : 170.f;
			spr->setPosition(pos + angle_relative_to(heading, angle) * scale);
		}
		else spr->setPosition(pos);

		// Set the origin
		spr->setOrigin(origin);

		// Rotate all the sprites except the aura sprite
		if (spr != &obj.spr_auras) spr->setRotation(obj.angle * RAD_TO_DEG);
	}
	return 1;
}

Player* Player::get_instance(lua_State* L)
{
	lua_getfield(L, -1, "ptr");
	Player* obj = (Player*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	return obj;
}

void Player::get_sprite(lua_State* L, int idx)
{
	// Verify the index (range 0-3)
	if (!(idx >= 0 && idx <= 3)) {
		lua_pushnil(L);
		return;
	}

	// Get the player object
	Player& obj = *get_instance(L);

	// Get and push the corresponding sprite
	sf::Sprite* sprites[] = { &obj.spr_auras, &obj.spr_cpits, &obj.spr_lwing, &obj.spr_rwing };
	lua_pushlightuserdata(L, sprites[idx]);
}

bool Player::load_texture(lua_State* L, int idx)
{
	// Check the index if it's valid or not (range 0-3)
	if (!(idx >= 0 && idx <= 3))
		return false;

	// Check the path argument
	if (lua_gettop(L) != 2 || !lua_isstring(L, -1)) return false;

	// Get the path argument
	std::string path = lua_tostring(L, -1);
	lua_pop(L, 1);

	// Get the player object
	Player& obj = *get_instance(L);

	// Load the texture
	sf::Texture tex;
	if (!tex.loadFromFile(path))
		return false;

	// Push it into the corresponding vectors
	Textures* arr[] = { &obj.tex_auras, &obj.tex_cpits, &obj.tex_lwing, &obj.tex_rwing };
	Textures& textures = *(arr[idx]);
	textures.push_back(tex);
	return true;
}

void Player::cycle_texture(lua_State* L, int idx)
{
	// Check the index if it's valid (0-3)
	if (!(idx >= 0 && idx <= 3)) return;

	// Get the argument (dir) if any
	int dir = 1;
	if (lua_gettop(L) == 2 && lua_isinteger(L, -1)) {
		dir = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	// Get the player instance
	Player& obj = *get_instance(L);

	// Get the corresponding sf::Texture array
	Textures* arr[] = { &obj.tex_auras, &obj.tex_cpits, &obj.tex_lwing, &obj.tex_rwing };
	Textures& textures = *arr[idx];

	// Increment the sprite index accordingly
	int* idxs[] = { &obj.idx_auras, &obj.idx_cpits, &obj.idx_lwing, &obj.idx_rwing };
	*idxs[idx] += dir;

	// Check the bounds
	if (*idxs[idx] < 0) *idxs[idx] = textures.size()-1;
	if (*idxs[idx] >= textures.size()) *idxs[idx] = 0;
}
