#ifndef __DYNAMICOBJECT_H__
#define __DYNAMICOBJECT_H__

#include "Vector3.h"
#include "GameObject.h"


class DynamicObject : public GameObject{
	
	protected: 
		Vector3 _speed;
		float _time;
		Vector3 _direction;

	public:
		DynamicObject();
		~DynamicObject();
    virtual void draw(){};
		void update(double delta_t);
		void setSpeed(const Vector3 &speed);
		void setSpeed(double x, double y, double z);
		Vector3 getSpeed();
		void setTime(float time);
		float getTime();
        virtual float* boxLimits() = 0;
        Vector3 getDirection();
        void setDirection(float x, float y, float z);
};

#endif /* __DYNAMICOBJECT_H__ */
