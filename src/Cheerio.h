#ifndef __CHEERIO_H__
#define __CHEERIO_H__

#include "DynamicObject.h"

class Cheerio : public DynamicObject {
	public:
		Cheerio(const Vector3 & pos, double xmin, double xmax, double zmin, double zmax);
		~Cheerio();
		void draw();
		float * boxLimits();
		void dealColision(const Vector3 & direction, float acceleration, float speed);
		void update(double delta_t);
private:
	float acceleration, speed, acceleration_factor, backwards_friction, backwards_friction_factor;
};

#endif /* __CHEERIO_H__ */
