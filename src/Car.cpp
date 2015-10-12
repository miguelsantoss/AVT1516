#include "Car.h"

Car::Car() {}

Car::Car(const Vector3& pos) {
	this->setPosition(pos);
}

Car::~Car() {}
void Car::draw() {}

float* Car::boxLimits() {
	return nullptr;
}
