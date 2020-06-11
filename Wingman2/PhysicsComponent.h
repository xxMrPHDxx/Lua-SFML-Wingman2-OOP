#pragma once

#include "pch.h"

class PhysicsComponent
{
private:
	sf::FloatRect hitbox;
	sf::Vector2f speed;
	float velocityMax = 20.f;
	float velocityMin = 2.0f;
	float acceleration = 160.f;
	float deceleration = 80.f;
	float drag = 0.98f;
	bool moving = false;
public:
	PhysicsComponent(float x, float y, float w, float h) : hitbox(x, y, w, h) {}
	~PhysicsComponent() {
		// Nothing to delete
	}

	void move(float, int, int);
	void update(float);

	static void register_class(lua_State*);

	static int create(lua_State*);
	static int destroy(lua_State*);

	static int get_position(lua_State*);
	static int get_size(lua_State*);
	static int get_speed(lua_State*);
	static int get_speed_limit(lua_State*);
	static int get_acceleration(lua_State*);
	static int get_deceleration(lua_State*);
	static int get_drag_factor(lua_State*);
	static int is_moving(lua_State*);

	static int move(lua_State*);
	static int update(lua_State*);
	static PhysicsComponent* get_instance(lua_State*);
};

