#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Entity.h"
#include "basic_geometry.h"
#include <vector>

class GameObject: public Entity {

	public:
		GameObject();
		GameObject(double xmin, double xmax, double zmin, double zmax);
		~GameObject();
    
		virtual void update(double delta_t){};
		double getXmin();
		double getXmax();
		double getZmin();
		double getZmax();

		double getOffsetX();
		double getOffsetZ();

		void setXmax(double xmax);

		void setXmin(double xmin);

		void setZmax(double ymax);

		void setZmin(double ymin);

		bool checkColision(GameObject * obj);

	private:
		double _xmin;
		double _xmax;
		double _zmin;
		double _zmax;
		double _x, _z;
};

#endif /* __GAMEOBJECT_H__ */
