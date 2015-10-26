#ifndef __DYNAMICOBJECT_H__
#define __DYNAMICOBJECT_H__

#include "Vector3.h"
#include "GameObject.h"


class DynamicObject : public GameObject{
	protected: 
		Vector3 _speed;
		float _time;
		Vector3 _direction;
		Vector3 _acceleration;
		float _distanceDone;
		Vector3 _previousPosition;

	public:
		DynamicObject();
		~DynamicObject();
		virtual void draw(){};
		void update(double delta_t);

		void setSpeed(const Vector3 &speed);
		void setSpeed(float x, float y, float z);
		void setTime(float time);
        void setDirection(float x, float y, float z);
		void setAcceleration(float x, float y, float z);
		void setAcceleration(const Vector3 &acceleration);

		virtual float* boxLimits() = 0;

		const Vector3& getDirection() const { return _direction; }
		const Vector3& getPreviousPosition() const { return _previousPosition; }
		const Vector3& getSpeed() const { return _speed; }
		const Vector3& getAcceleration() const { return _acceleration; }
		void setDistanceDone(float distanceDone) { _distanceDone = distanceDone; }
		float getDistanceDone() { return _distanceDone; }
		const float getTime() const { return _time; }
};

#endif /* __DYNAMICOBJECT_H__ */
