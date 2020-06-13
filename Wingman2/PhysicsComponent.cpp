#include "pch.h"
#include "PhysicsComponent.h"

void PhysicsComponent::move(float dt, int dir_x, int dir_y)
{
    // Set the moving flag
    this->moving = true;

    // Update acceleration depending on direction
    if(dir_x != 0) this->speed.x += dt * (dir_x > 0 ? 1 : -1) * this->acceleration;
    if(dir_y != 0) this->speed.y += dt * (dir_y > 0 ? 1 : -1) * this->acceleration;

    // Limit velocity
    if (magnitude(this->speed) > this->velocityMax) this->speed = normalize(this->speed) * this->velocityMax;
}

void PhysicsComponent::update(float dt)
{
    // Apply the speed
    this->hitbox.left += this->speed.x * dt;
    this->hitbox.top  += this->speed.y * dt;

    // std::cout << "Moving (" << this->speed.x << ", " << this->speed.y << ")" << std::endl;

    // Update deceleration
    this->speed *= this->drag;
    if (!this->moving && magnitude(this->speed) < this->velocityMin)
        this->speed = sf::Vector2f(0, 0);

    // Remove the moving flag
    this->moving = false;
}

void PhysicsComponent::register_class(lua_State* L)
{
    // Create a new metatable of PhysicsComponent
    lua_newtable(L);

    // Set the index
    lua_pushstring(L, "PhysicsComponent");
    lua_setfield(L, -2, "__index");

    // Static method to create new object
    lua_pushcfunction(L, create);
    lua_setfield(L, -2, "new");

    // Set as global
    lua_setglobal(L, "PhysicsComponent");
}

int PhysicsComponent::create(lua_State* L)
{
    // Check the 4 arguments
    float x, y, w, h;
    if (lua_gettop(L) == 5) {
        bool okay = true;
        for (int i = 1; i <= 4; i++) {
            if (!lua_isnumber(L, -i) && !lua_isinteger(L, -1)) {
                okay = false;
                break;
            }
        }
        if (!okay) {
            lua_pushnil(L);
            return 1;
        }
    }

    // Get the 4 arguments
    float* ptr[] = { &x, &y, &w, &h };
    for (int j, i = 1; i <= 4; i++) {
        j = i - 5;
        *ptr[i - 1] = float(lua_isnumber(L, j) ? lua_tonumber(L, j) : lua_tointeger(L, j));
    }
    lua_pop(L, 4);

    // Create empty table for our PhysicsComponent object
    lua_newtable(L);

    // Set member methods
    luaL_Reg methods[] = {
        { "destroy", destroy },
        { "set_position", set_position },
        { "get_position", get_position },
        { "get_size", get_size },
        { "get_speed", get_speed },
        { "get_speed_limit", get_speed_limit },
        { "get_acceleration", get_acceleration },
        { "get_deceleration", get_deceleration },
        { "get_drag", get_drag_factor },
        { "move", move }
    };
    for (luaL_Reg& reg : methods) {
        lua_pushcfunction(L, reg.func);
        lua_setfield(L, -2, reg.name);
    }

    // Set the object pointer
    lua_pushlightuserdata(L, new PhysicsComponent(x, y, w, h));
    lua_setfield(L, -2, "ptr");

    // Set the metatable
    lua_getglobal(L, "PhysicsComponent");
    lua_setmetatable(L, -2);
    return 1;
}

int PhysicsComponent::destroy(lua_State* L)
{
    PhysicsComponent* obj = get_instance(L);
    delete obj;
    return 1;
}

int PhysicsComponent::set_position(lua_State* L)
{
    // Check the argument
    if (lua_gettop(L) != 3 || !lua_isnumber(L, -2) || !lua_isnumber(L, -1)) {
        return 1;
    }

    // Get the argument
    float x = lua_tonumber(L, -2);
    float y = lua_tonumber(L, -1);
    lua_pop(L, 2);

    // Get the object
    PhysicsComponent& obj = *get_instance(L);

    // Set the position
    obj.hitbox.left = x;
    obj.hitbox.top = y;
    return 1;
}

int PhysicsComponent::get_position(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_newtable(L);
    lua_pushnumber(L, obj.hitbox.left);
    lua_seti(L, -2, 1);
    lua_pushnumber(L, obj.hitbox.top);
    lua_seti(L, -2, 2);
    return 1;
}

int PhysicsComponent::get_size(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_newtable(L);
    lua_pushnumber(L, obj.hitbox.width);
    lua_seti(L, -2, 1);
    lua_pushnumber(L, obj.hitbox.height);
    lua_seti(L, -2, 2);
    return 1;
}

int PhysicsComponent::get_speed(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_newtable(L);
    lua_pushnumber(L, obj.speed.x);
    lua_seti(L, -2, 1);
    lua_pushnumber(L, obj.speed.y);
    lua_seti(L, -2, 2);
    return 1;
}

int PhysicsComponent::get_speed_limit(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_newtable(L);
    lua_pushnumber(L, obj.velocityMin);
    lua_seti(L, -2, 1);
    lua_pushnumber(L, obj.velocityMax);
    lua_seti(L, -2, 2);
    return 1;
}

int PhysicsComponent::get_acceleration(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_pushnumber(L, obj.acceleration);
    return 1;
}

int PhysicsComponent::get_deceleration(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_pushnumber(L, obj.deceleration);
    return 1;
}

int PhysicsComponent::get_drag_factor(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_pushnumber(L, obj.drag);
    return 1;
}

int PhysicsComponent::is_moving(lua_State* L)
{
    PhysicsComponent& obj = *get_instance(L);
    lua_pushboolean(L, obj.moving);
}

int PhysicsComponent::move(lua_State* L)
{
    // Check the direction arguments
    if (lua_gettop(L) != 3 || !lua_isinteger(L, -2) || !lua_isinteger(L, -1)) {
        return 1;
    }
    
    // Get the arguments
    sf::Vector2i dir(lua_tointeger(L, -2), lua_tointeger(L, -1));
    lua_pop(L, 2);

    // Get the global dt, if not available default to 1
    lua_getglobal(L, "dt");
    float dt = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 0.0016f;
    lua_pop(L, 1);

    std::cout << "Delta time: " << dt << std::endl;

    // Get the object and move
    PhysicsComponent& obj = *get_instance(L);
    obj.move(dt, dir.x, dir.y);
    return 1;
}

int PhysicsComponent::update(lua_State* L)
{
    // Get the object and update
    PhysicsComponent& obj = *get_instance(L);
    return 1;
}

PhysicsComponent* PhysicsComponent::get_instance(lua_State* L)
{
    lua_getfield(L, -1, "ptr");
    PhysicsComponent* obj = (PhysicsComponent*) lua_touserdata(L, -1);
    lua_pop(L, 1);
    return obj;
}
