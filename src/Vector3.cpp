#include "Vector3.h"

Vector3::Vector3() {
	_x=0.0;
	_y=0.0;
	_z=0.0;
}

Vector3::Vector3(double x, double y, double z) {
	_x=x;
	_y=y;
	_z=z;
}

Vector3::~Vector3() {}

double Vector3::getX() const {
		return _x;
}

double Vector3::getY() const {
		return _y;
}

double Vector3::getZ() const {
		return _z;
}

void Vector3::setX(double x) {
	_x = x;
}

void Vector3::setY(double y) {
	_y = y;
}

void Vector3::setZ(double z) {
	_z = z;
}

void Vector3::set(double x, double y, double z) {
	_x=x;
	_y=y;
	_z=z;
}

void Vector3::print() {
	printf("(%f, %f, %f)\n", _x, _y, _z);
}