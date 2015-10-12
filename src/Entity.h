
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector3.h"

class Entity {
	protected:
		Vector3 _position;
	public:
		Entity();
		virtual ~Entity();
		
		const Vector3& getPosition() const { return _position; }
		void setPosition(float x, float y, float z);
		void setPosition(const Vector3& position);
};

#endif /* __ENTITY_H__ */