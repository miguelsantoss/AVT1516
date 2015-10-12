#include "Vector4.h"

Vector4::Vector4() :Vector3() {
	_w = 0.0;
}

Vector4::Vector4(float in_x, float in_y, float in_z, float in_w) :Vector3(in_x, in_y, in_z) {
	_w = in_w;
}

Vector4::~Vector4() {
}

void Vector4::set(float in_x, float in_y, float in_z, float in_w) {
	Vector3::set(in_x, in_y, in_z);
	_w = in_w;
}

float Vector4::getW() {
	return _w;
}