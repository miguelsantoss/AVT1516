#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Entity.h"
#include "basic_geometry.h"
#include <vector>

class GameObject: public Entity {

	public:
		GameObject();
		~GameObject();
    
		virtual void update(double delta_t){};
	private:
};

#endif /* __GAMEOBJECT_H__ */
