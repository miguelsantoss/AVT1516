#include "Orange.h"
#include "Vector3.h"
#include "GameManager.h"

Orange::Orange() {}

Orange::Orange(const Vector3& pos) : DynamicObject(pos, 0.4,0.4){
	interval = 5;
	reset(pos,1);
	_angle = 0;
}
Orange::~Orange() {}

void Orange::draw() {}

float* Orange::boxLimits() {
	return nullptr;
}

void Orange::increaseSpeed() {
	interval += 1;
}
void Orange::reset(const Vector3& pos, double elapsed) {
	float x = pos.getX();
	float z = pos.getZ();
	this->setPosition(pos);
	Vector3 speed = Vector3((rand() % interval - interval / 2) / 2000.0, 0, (rand() % interval - interval / 2) / 2000.0);
	while (1) {
		if (speed.getX() == 0 && speed.getZ() == 0)
			speed = Vector3((rand() % interval - interval / 2) / 2000.0, 0, (rand() % interval - interval / 2) / 2000.0);
		else
			break;
	}
	this->setSpeed(speed.getX(), speed.getY(), speed.getZ());
	this->setDirection(_speed.getX() / abs(_speed.getX()), _speed.getY() / abs(_speed.getY()), _speed.getZ() / abs(_speed.getZ()));
	float *a = _speed.getArray();
	float length = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / length;
	a[1] = a[1] / length;
	a[2] = a[2] / length;
	this->setDirection(a[0], a[1], a[2]);
	//this->setSpeed(0.001, 0, 0.00001);
}