#include "ButterPacket.h"

ButterPacket::ButterPacket(const Vector3 & pos, double xmin, double xmax, double zmin, double zmax) :
	DynamicObject(pos, xmin, xmax, zmin, zmax) {
	acceleration = 0;
	speed = 0;
	acceleration_factor = 0.00002;
	backwards_friction_factor = 0.003;
	this->setDirection(1.0, 0.0, 0.0);
}

ButterPacket::~ButterPacket() {}

void ButterPacket::draw() {}

void ButterPacket::dealColision(const Vector3 &direction, float acceleration, float speed) {
	if (speed > 0.004 || speed < -0.004) {
		this->setDirection(direction.getX(), direction.getY(), direction.getZ());
		this->speed = speed;
	}
}
void ButterPacket::update(double delta_t) {
	Vector3 direction = this->getDirection();

	double x = this->getDirection().getX();
	double z = this->getDirection().getZ();
	backwards_friction = -speed * backwards_friction_factor;
	speed = speed + backwards_friction * delta_t;
	speed = speed + acceleration* delta_t;

	Vector3 position = this->getPosition();

	Vector3* lastposition = new Vector3(position.getX(), position.getY(), position.getZ());
	setSpeed(speed * _direction.getX(), speed * direction.getY(), speed * direction.getZ());
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
float* ButterPacket::boxLimits() {
	return nullptr;
}