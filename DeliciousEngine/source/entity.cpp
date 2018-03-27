#include "entity.h"

void Entity::Load() {
	lua_ref["load"](lua_ref);
}

void Entity::Begin() {
	lua_ref["begin"](lua_ref);
}

void Entity::Update() {
	lua_ref["update"](lua_ref);
}

void Entity::End() {
	lua_ref["end"](lua_ref);
}

