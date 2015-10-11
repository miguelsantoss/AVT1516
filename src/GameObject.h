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
		std::vector<struct MyMesh>* getMesh() { return &mesh;  }
	private:
		std::vector<struct MyMesh> mesh;
};

#endif /* __GAMEOBJECT_H__ */
