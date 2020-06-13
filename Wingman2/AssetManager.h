#pragma once

#include "pch.h"

typedef std::vector<sf::Texture> Textures;

class AssetManager
{
private:
	Textures textures;
public:
	static void register_class(lua_State*);

	static int create(lua_State*);
	static int destroy(lua_State*);

	static int get_texture(lua_State*);

	static int load_texture(lua_State*);
	static int get_instance(lua_State*);
};