#include "Car.h"

Car::Car() {}

Car::Car(const Vector3& pos) {
	acceleration_factor = 0.0001;
	acceleration_input = 0;
	steer_angle = 0; 
	steer_input = 0;
	steer_factor = 1;
	current_speed = 0;
	max_speed = 0.06;
	backwards_friction_factor = 0.0001;

	this->setPosition(pos);
}

Car::~Car() {}
void Car::draw() {}

float* Car::boxLimits() {
	return nullptr;
}

void Car::accelerationIncrease() {
	acceleration_input += 0.2;
	if (acceleration_input > 1) {
		acceleration_input = 1;
	}
}
void Car::accelerationDecrease() {
	acceleration_input -= 0.2;
	if (acceleration_input < -1) {
		acceleration_input = -1;
	}
}

void Car::steerLeft() {
	steer_input = -M_PI/12; //FIXME
}
void Car::steerRight() {
	steer_input = M_PI/12; //FIXME
}
void Car::steerStop() {
	steer_input = 0;
}

Vector3 lerp(double t, Vector3 a, Vector3 b) {
	return a*(1 - t) + b*t;
}

void Car::update(double delta_t){
	//if(getTime() > 0) return;
	Vector3 direction = this->getDirection();

	setAcceleration(direction * acceleration_input * acceleration_factor);
	steer_angle = steer_input * steer_factor;
	double x = this->getDirection().getX();
	double z = this->getDirection().getZ();
	Vector3 newDirection = Vector3(x * cos(steer_angle) - z * sin(steer_angle), 0.0, x * sin(steer_angle) + z * cos(steer_angle));
	_direction = newDirection;
	//_direction = lerp(delta_t/100, getDirection(), newDirection);
	steer_input = 0;

	Vector3 speed = this->getSpeed();
	backwards_friction = Vector3(0,0,0)-speed * backwards_friction_factor;
	
	//backwards_friction = backwards_friction.resize(std::min(backwards_friction.norm(), speed.norm() / delta_t));

	setSpeed(speed + backwards_friction * delta_t);
	speed = this->getSpeed();
	current_speed = speed.norm();
	//if (current_speed < max_speed)
	//{
		this->setSpeed(speed.getX() + delta_t * _acceleration.getX(), speed.getY() + delta_t * _acceleration.getY(), speed.getZ() + delta_t * _acceleration.getZ());
	//}	
	Vector3 position = this->getPosition();
	Entity::setPosition(position.getX() + delta_t * _speed.getX(), position.getY() + delta_t * _speed.getY(), position.getZ() + delta_t * _speed.getZ());
}
