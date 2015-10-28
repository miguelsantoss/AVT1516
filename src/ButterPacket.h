#ifndef __BUTTERPACKET_H__
#define __BUTTERPACKET_H__

#include "DynamicObject.h"

class ButterPacket : public DynamicObject {
	public:
		ButterPacket(const Vector3 & pos, double xmin, double xmax, double zmin, double zmax);
		~ButterPacket();
		void draw();
		void dealColision(const Vector3 & direction, float acceleration, float speed);
		void update(double delta_t);
		float* boxLimits();
private:
	float acceleration, speed, acceleration_factor, backwards_friction, backwards_friction_factor;
};

#endif /* __BUTTERPACKET_H__ */
