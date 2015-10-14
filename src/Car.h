#ifndef __CAR_H__
#define __CAR_H__
#define _USE_MATH_DEFINES

#include <math.h>
#include <algorithm>

#include "DynamicObject.h"


class Car: public DynamicObject {
	public:
		Car();
		Car(const Vector3& pos);
		~Car();
		void draw();
		void accelerationIncrease();
		void accelerationDecrease();
		void steerRight();
		void steerLeft();
		void steerStop();
		void update(double delta_t) override;
		float* boxLimits();
private:
	double acceleration_input;
	double acceleration_factor;
	double steer_angle, steer_input, steer_factor;
	double current_speed, max_speed;
	Vector3 backwards_friction;
    double backwards_friction_factor;
};

#endif /* __CAR_H__ */
