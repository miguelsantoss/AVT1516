#include "Entity.h"
#include <iostream>

Entity::Entity() {
	_position = new Vector3();
}

Entity::~Entity() {
	delete _position;
}

Vector3* Entity::getPosition() {
	return _position;
}

Vector3* Entity::setPosition(double x, double y, double z) {
	_position = new Vector3(x, y, z);
	return _position;
}

Vector3* Entity::setPosition(Vector3* position) {
	_position = new Vector3(position->getX(), position->getY(), position->getZ());
	return _position;
}