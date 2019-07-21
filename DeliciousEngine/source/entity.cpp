#include "entity.h"

#include "console.h"

void Entity::load() {
	
}
void Entity::begin() {
	
}
void Entity::update() {
	if (logic) logic(this);
}
void Entity::end() {
	
}