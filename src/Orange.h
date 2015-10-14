#ifndef __ORANGE_H__
#define __ORANGE_H__

#include <stdlib.h>
#include "DynamicObject.h"
#include "Vector3.h"


class Orange  : public DynamicObject {
	public:
		Orange();
		Orange(const Vector3 & pos);
		~Orange();
		void draw();
		void reset(const Vector3& pos, double elapsed);
		float* Orange::boxLimits();
		void increaseSpeed();
private:
	int interval;
};

#endif /* __ORANGE_H__ */
