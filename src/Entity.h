
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector3.h"

class Entity {
	protected:
		Vector3* _position;
	public:
		Entity();
		virtual ~Entity();
		
		Vector3* getPosition();
		Vector3* setPosition(double x, double y, double z);
		Vector3* setPosition(Vector3* position);
};

#endif /* __ENTITY_H__ */