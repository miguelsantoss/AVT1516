#include "Vector3.h"

Vector3::Vector3() {
	_x=0.0;
	_y=0.0;
	_z=0.0;
}

Vector3::Vector3(float x, float y, float z) {
	_x=x;
	_y=y;
	_z=z;
}

Vector3::~Vector3() {}

float Vector3::getX() const {
		return _x;
}

float Vector3::getY() const {
		return _y;
}

float Vector3::getZ() const {
		return _z;
}

void Vector3::setX(float x) {
	_x = x;
}

void Vector3::setY(float y) {
	_y = y;
}

void Vector3::setZ(float z) {
	_z = z;
}

void Vector3::set(float x, float y, float z) {
	_x=x;
	_y=y;
	_z=z;
}

void Vector3::print() {
	printf("(%f, %f, %f)\n", _x, _y, _z);
}