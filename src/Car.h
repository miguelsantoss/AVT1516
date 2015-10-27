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
		void accelerationStop();
		void steerRight();
		void steerLeft();
		void steerStop();
		void update(double delta_t) override;
		float* boxLimits();
		double getAngle();
		void setAngle(double angle) { _angle = angle; }
		double getWeelAngle();
private:
	double acceleration, speed;
	double acceleration_input;
	double acceleration_factor;
	double steer_angle, steer_input, steer_factor, _angle, weel_angle;
	double current_speed, max_speed;
    double backwards_friction_factor, backwards_friction;
};

#endif /* __CAR_H__ */
