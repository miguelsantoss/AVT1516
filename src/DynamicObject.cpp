#include "DynamicObject.h"
#include "Entity.h"
#include "Vector3.h"
#include "stdio.h"

DynamicObject::DynamicObject() {
	_time = 0;
}

DynamicObject::~DynamicObject() {}

void DynamicObject::setTime(float time) {
	_time = time;
}

float DynamicObject::getTime() {
	return _time;
}

void DynamicObject::setSpeed(double x, double y, double z) {
	_speed.set(x, y, z);
}

void DynamicObject::setSpeed(const Vector3 &speed) {
	_speed.set(speed.getX(), speed.getY(), speed.getZ());
}

void DynamicObject::update(double delta_t) {
		if(getTime() > 0) return;
		Vector3 *position = Entity::getPosition();

		Entity::setPosition(position->getX() + delta_t * _speed.getX(), position->getY() + delta_t * _speed.getY(), position->getZ() + delta_t * _speed.getZ());
}

Vector3 DynamicObject::getSpeed() {
	return _speed;
}

Vector3 DynamicObject::getDirection() {
	return _direction;
}

void DynamicObject::setDirection(float x, float y, float z) {
	_direction.set(x, y, z);
}