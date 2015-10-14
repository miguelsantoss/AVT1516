#include "Car.h"

Car::Car() {}

Car::Car(const Vector3& pos) {
	acceleration_factor = 0.00002;
	acceleration_input = 0;
	steer_angle = 0; 
	steer_input = 0;
	steer_factor = 1;
	current_speed = 0;
	max_speed = 0.06;
	backwards_friction_factor = 0.004;
	this->setDirection(1.0, 0.0, 0.0);
	this->setPosition(pos);
}

Car::~Car() {}
void Car::draw() {}

float* Car::boxLimits() {
	return nullptr;
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
	steer_input = -M_PI/24; //FIXME
}
void Car::steerRight() {
	steer_input = M_PI/24; //FIXME
}
void Car::steerStop() {
	steer_input = 0;
}

void Car::update(double delta_t){
	//if(getTime() > 0) return;
	Vector3 direction = this->getDirection();
	setAcceleration(direction * acceleration_input * acceleration_factor);
	steer_angle = steer_input * steer_factor;
	double x = this->getDirection().getX();
	double z = this->getDirection().getZ();
	_direction = Vector3(x * cos(steer_angle) - z * sin(steer_angle), 0.0, x * sin(steer_angle) + z * cos(steer_angle));

	Vector3 speed = this->getSpeed();
	backwards_friction = Vector3(-speed.getX(), -speed.getY(), -speed.getZ()) * backwards_friction_factor;
	speed = this->getSpeed();
	setSpeed(speed + backwards_friction * delta_t);

	speed = this->getSpeed();
	this->setSpeed(speed.getX() + (delta_t * _acceleration.getX()), speed.getY() + (delta_t * _acceleration.getY()), speed.getZ() + (delta_t * _acceleration.getZ()));
	Vector3 position = this->getPosition();
	Entity::setPosition(position.getX() + (delta_t * _speed.getX()), position.getY() + (delta_t * _speed.getY()), position.getZ() + (delta_t * _speed.getZ()));
}
