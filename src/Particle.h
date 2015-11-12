#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Vector3.h"
#include "Entity.h"


class Particle {

protected:
	float _life;
	float _fade;
	Vector3 _color;
	Vector3 _position;
	Vector3 _speed;
	Vector3 _acceleration;

	
public:
	Particle(float pos_x, float pos_y, float pos_z, 
			 float speed_x, float speed_y, float speed_z,
			 float acc_x, float acc_y, float acc_z,
			 float color_r, float color_g, float color_b,
			 float life, float fade);
	Particle();
	virtual ~Particle();
	void update(float delta_t);
	void resetValues(float pos_x, float pos_y, float pos_z,
					 float speed_x, float speed_y, float speed_z,
					 float acc_x, float acc_y, float acc_z,
					 float color_r, float color_g, float color_b,
					 float life, float fade);

	float getLife() { return _life; }
	const Vector3& getColor() { return _color; }
	const Vector3& getPosition() { return _position; }
};





#endif /* __PARTICLE_H__ */
