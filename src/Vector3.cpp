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

double Vector3::norm() {
	return sqrt(_x*_x + _y*_y + _z*_z);
}

Vector3 Vector3::resize(double r) {
	if (_x > r) _x = r;
	if (_y > r) _x = r;
	if (_z > r) _x = r;
	return *this;
}

void Vector3::print() {
	printf("(%f, %f, %f)\n", _x, _y, _z);
}


Vector3 Vector3::operator=(const Vector3& vec) {
	_x = vec.getX();
	_y = vec.getY();
	_z = vec.getZ();
	return *this;
}

Vector3 Vector3::operator*(double num) {
	_x *= num;
	_y *= num;
	_z *= num;
	return *this;
}

Vector3 Vector3::operator/(int num) {
	_x /= num;
	_y /= num;
	_z /= num;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& vec) {
	_x += vec.getX();
	_y += vec.getY();
	_z += vec.getZ();
	return *this;
}

Vector3 Vector3::operator-(const Vector3& vec) {
	_x -= vec.getX();
	_y -= vec.getY();
	_z -= vec.getZ();
	return *this;
}