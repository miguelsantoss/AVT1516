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

void DynamicObject::setSpeed(float x, float y, float z) {
	_speed.set(x, y, z);
}

void DynamicObject::setSpeed(const Vector3 &speed) {
	_speed.set(speed.getX(), speed.getY(), speed.getZ());
}

void DynamicObject::setAcceleration(float x, float y, float z) {
	_acceleration.set(x, y, z);
}

void DynamicObject::setAcceleration(const Vector3 &acceleration) {
	_acceleration.set(acceleration.getX(), acceleration.getY(), acceleration.getZ());
}

void DynamicObject::update(double delta_t) {
		//if(getTime() > 0) return;
		
		Vector3 position = this->getPosition();
		Vector3 speed = this->getSpeed();
		this->setSpeed(speed.getX() + delta_t * _acceleration.getX(), speed.getY() + delta_t * _acceleration.getY(), speed.getZ() + delta_t * _acceleration.getZ());
		Entity::setPosition(position.getX() + delta_t * _speed.getX(), position.getY() + delta_t * _speed.getY(), position.getZ() + delta_t * _speed.getZ());
}

void DynamicObject::setDirection(float x, float y, float z) {
	_direction.set(x, y, z);
}