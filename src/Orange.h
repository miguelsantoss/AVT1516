#ifndef __ORANGE_H__
#define __ORANGE_H__

#include "DynamicObject.h"
#include "Vector3.h"

class Orange  : public DynamicObject {
	public:
		Orange();
		~Orange();
		void draw();
};

#endif /* __ORANGE_H__ */
