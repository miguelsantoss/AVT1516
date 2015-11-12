#include "Particle.h"
#include <iostream>

Particle::Particle( float pos_x, float pos_y, float pos_z,
					float speed_x, float speed_y, float speed_z,
					float acc_x, float acc_y, float acc_z,
					float color_r, float color_g, float color_b,
					float life, float fade) {
	_life = life;
	_fade = fade;
	_position = Vector3(pos_x, pos_y, pos_z);
	_speed = Vector3(speed_x, speed_y, speed_z);
	_acceleration = Vector3(acc_x, acc_y, acc_z);
	_color = Vector3(color_r, color_g, color_b);	
}

Particle::Particle() {}

void Particle::resetValues(float pos_x, float pos_y, float pos_z,
					  float speed_x, float speed_y, float speed_z,
					  float acc_x, float acc_y, float acc_z,
					  float color_r, float color_g, float color_b,
					  float life, float fade) {
	_life = life;
	_fade = fade;

	
	_position.set(pos_x, pos_y, pos_z);
	_speed.set(speed_x, speed_y, speed_z);
	_acceleration.set(acc_x, acc_y, acc_z);
	_color.set(color_r, color_g, color_b);
	
}

Particle::~Particle() {}

void Particle::update(float delta_t) {
	_position.setX(_position.getX() + (_speed.getX() * delta_t));
	_position.setY(_position.getY() + (_speed.getY() * delta_t));
	_position.setZ(_position.getZ() + (_speed.getZ() * delta_t));

	_speed.setX(_speed.getX() + (_acceleration.getX() * delta_t));
	_speed.setY(_speed.getY() + (_acceleration.getY() * delta_t));
	_speed.setZ(_speed.getZ() + (_acceleration.getZ() * delta_t));

	_life -= _fade;
}
