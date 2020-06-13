#pragma once

#include "pch.h"

#include "PhysicsComponent.h"

typedef std::vector<sf::Texture> Textures;

class Player
{
private:
	Textures tex_auras, tex_cpits, tex_lwing, tex_rwing;
	sf::Sprite spr_auras, spr_cpits, spr_lwing, spr_rwing;
	int idx_auras, idx_cpits, idx_lwing, idx_rwing;

	float angle = 0.f;
public:
	Player() {
		idx_auras = idx_cpits = idx_lwing = idx_rwing = 0;
	}
	~Player() {
		// Nothing to delete
	}

	static void register_class(lua_State*);

	static int create(lua_State*);
	static int destroy(lua_State*);

	static int set_angle(lua_State*);
	static int set_sprite_scale(lua_State*);
	static int get_auras_sprite(lua_State*);
	static int get_cpits_sprite(lua_State*);
	static int get_lwing_sprite(lua_State*);
	static int get_rwing_sprite(lua_State*);

	static int load_auras_texture(lua_State*);
	static int load_cpits_texture(lua_State*);
	static int load_lwing_texture(lua_State*);
	static int load_rwing_texture(lua_State*);
	static int cycle_auras(lua_State*);
	static int cycle_cpits(lua_State*);
	static int cycle_lwing(lua_State*);
	static int cycle_rwing(lua_State*);
	static int move(lua_State*);
	static int update(lua_State*);
	static Player* get_instance(lua_State*);

private:
	static void get_sprite(lua_State*, int);
	static bool load_texture(lua_State*, int);
	static void cycle_texture(lua_State*, int);
};

