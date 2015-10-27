#include "Car.h"
#include <iostream>

Car::Car() {}

Car::Car(const Vector3& pos) : DynamicObject(pos, -0.35, 0.35, -0.5, 0.5) {
	acceleration = 0;
	speed = 0;
	acceleration_factor = 0.00002;
	acceleration_input = 0;
	_angle = 0;
	weel_angle = 0;
	steer_angle = 1; 
	steer_input = 0;
	steer_factor = M_PI/4;
	current_speed = 0;
	max_speed = 0.06;
	backwards_friction_factor = 0.004;
	this->setDirection(1.0, 0.0, 0.0);
	//this->setPosition(pos);
	
}

Car::~Car() {}
void Car::draw() {}

float* Car::boxLimits() {
	return nullptr;
}

double Car::getAngle() {
	return _angle;
}

double Car::getWeelAngle() {
	return weel_angle;
}

void Car::accelerationIncrease() {
	acceleration_input = 1;
}
void Car::accelerationDecrease() {
	acceleration_input = -1;
}

void Car::accelerationStop() {
	acceleration_input = 0;
}
void Car::steerLeft() {
		steer_input = -5; //FIXME
		weel_angle = 25;
}
void Car::steerRight() {
		steer_input = 5; //FIXME
		weel_angle = -25;
}
void Car::steerStop() {
	steer_input = 0;
	weel_angle = 0;
}

void Car::update(double delta_t){
	Vector3 direction = this->getDirection();
	acceleration =  acceleration_input * acceleration_factor;

	if (speed >= 0) {
		steer_angle = steer_input * 0.0174532925;
		_angle = _angle - steer_input;
	}
	else {
		steer_angle = -steer_input * 0.0174532925;
		_angle = _angle + steer_input;
	}
	double x = this->getDirection().getX();
	double z = this->getDirection().getZ();
	setDirection(x * cos(steer_angle) - z * sin(steer_angle), 0.0, x * sin(steer_angle) + z * cos(steer_angle));
	backwards_friction = -speed * backwards_friction_factor;
	speed = speed + backwards_friction * delta_t;
	speed = speed + acceleration* delta_t;

	Vector3 position = this->getPosition();

	Vector3* lastposition = new Vector3(position.getX(), position.getY(), position.getZ());

	double pos_x = position.getX() + (delta_t * speed * _direction.getX());
	double pos_y = position.getY() + (delta_t * speed * _direction.getY());
	double pos_z = position.getZ() + (delta_t * speed* _direction.getZ());
	setXmax(pos_x + getOffsetX());
	setXmin(pos_x - getOffsetX());
	setZmax(pos_z + getOffsetZ());
	setZmin(pos_z - getOffsetZ());

	Entity::setPosition(pos_x, pos_y, pos_z);	float newx, newy, newz;

	position = this->getPosition();
	newx = position.getX() - lastposition->getX();
	newx *= newx;
	newy = position.getY() - lastposition->getY();
	newy *= newy;
	newz = position.getZ() - lastposition->getZ();
	newz *= newz;
	this->setDistanceDone(sqrt(newx + newy + newz));
}
