#include "Entity.h"
#include <iostream>

Entity::Entity() {}

Entity::~Entity() {}

void Entity::setPosition(float x, float y, float z) {
	_position.set(x, y, z);
}

void Entity::setPosition(const Vector3& position) {
	_position.set(position.getX(), position.getY(), position.getZ());
}