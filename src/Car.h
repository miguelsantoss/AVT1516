#ifndef __CAR_H__
#define __CAR_H__

#include "DynamicObject.h"

class Car: public DynamicObject {
	public:
		Car();
		~Car();
		void draw();
		float* boxLimits();
};

#endif /* __CAR_H__ */