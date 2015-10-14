#include "Orange.h"
#include "Vector3.h"
#include "GameManager.h"

Orange::Orange() {}

Orange::Orange(const Vector3& pos) {
	interval = 5;
	reset(pos,1);
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
	this->setPosition(pos);
	this->setSpeed((rand() % interval - interval/2)/10000.0, 0, (rand() % interval - interval/2)/10000.0);
	//this->setSpeed(0.001, 0, 0.00001);
}