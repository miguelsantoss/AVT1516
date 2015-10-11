#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Entity.h"

class GameObject: public Entity {

	public:
		GameObject();
		~GameObject();
    
		virtual void update(double delta_t){};
};

#endif /* __GAMEOBJECT_H__ */
