#include "Cheerio.h"

Cheerio::Cheerio(const Vector3 & pos, double xmin, double xmax, double zmin, double zmax) : 
	DynamicObject(pos, xmin, xmax, zmin, zmax) {
	acceleration = 0;
	speed = 0;
	acceleration_factor = 0.00002;
	backwards_friction_factor = 0.006;
	this->setDirection(1.0, 0.0, 0.0);
}

Cheerio::~Cheerio() {}

void Cheerio::draw() {}

float* Cheerio::boxLimits() {
	return nullptr;
}

void Cheerio::dealColision(const Vector3 &direction, float acceleration, float speed) {
	if (speed > 0.0035 || speed < -0.0035) {
		this->setDirection(direction.getX(), direction.getY(), direction.getZ());
		this->speed = speed;
	}
}
void Cheerio::update(double delta_t) {
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