#pragma once

#include "pch.h"

#include "PhysicsComponent.h"

typedef std::vector<sf::Texture> Textures;

class Player
{
private:
	Textures tex_auras, tex_cpits, tex_lwing, tex_rwing;
	sf::Sprite spr_auras, spr_cpits, spr_lwing, spr_rwing;
	float angle = 0.f;
public:
	Player() {}
	~Player() {
		// Nothing to delete
	}

	static void register_class(lua_State*);

	static int create(lua_State*);
	static int destroy(lua_State*);

	static int move(lua_State*);
	static int update(lua_State*);
	static Player* get_instance(lua_State*);
};

