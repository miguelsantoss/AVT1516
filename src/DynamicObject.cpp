#include "DynamicObject.h"
#include "Entity.h"
#include "Vector3.h"
#include "stdio.h"

DynamicObject::DynamicObject() {
	_time = 0;
	_distanceDone = 0;
}

DynamicObject::DynamicObject(const Vector3& pos, float offsetX, float offsetY) : GameObject(offsetX, offsetY)
{
	setXmax(pos.getX() + getOffsetX());
	setXmin(pos.getX() - getOffsetX());
	setZmax(pos.getZ() + getOffsetZ());
	setZmin(pos.getZ() - getOffsetZ());
	this->setPosition(pos);
	this->setAcceleration(0, 0, 0);
	this->setDirection(1, 0, 0);
	this->setSpeed(0.0, 0, 0);
	_distanceDone = 0;
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

Vector3 DynamicObject::rotateCoordinate(float x, float y, float centerX, float centerY, float angle) {
	float x2 = centerX + (x - centerX)*cos(angle) + (y - centerY)*sin(angle);
	float y2 = centerY - (x - centerX)*sin(angle) + (y - centerY)*cos(angle);
	return Vector3(x, y, 0);
}

void DynamicObject::dealColision(const Vector3 &acceleration) {
	setAcceleration(acceleration);
}

void DynamicObject::update(double delta_t) {
		//if(getTime() > 0) return;
		
		Vector3 position = this->getPosition();
		Vector3 speed = this->getSpeed();
		_previousPosition.setX(position.getX());
		_previousPosition.setY(position.getY());
		_previousPosition.setZ(position.getZ());
		this->setSpeed(speed.getX() + delta_t * _acceleration.getX(), speed.getY() + delta_t * _acceleration.getY(), speed.getZ() + delta_t * _acceleration.getZ());
		
		speed = this->getSpeed(); 
		double x = position.getX() + delta_t * _speed.getX();
		double z = position.getZ() + delta_t * _speed.getZ();
		setXmax(x + getOffsetX());
		setXmin(x - getOffsetX());
		setZmax(z + getOffsetZ());
		setZmin(z - getOffsetZ());
		Entity::setPosition(x, position.getY() + delta_t * _speed.getY(), z);
		_distanceDone = delta_t * abs(_speed.getX()) + delta_t * abs(_speed.getZ());
}

void DynamicObject::setDirection(float x, float y, float z) {
	_direction.set(x, y, z);
}