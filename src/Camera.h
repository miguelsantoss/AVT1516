
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3.h"
#include "Entity.h"


class Camera: public Entity {

	protected:
			Vector3 _up;
			Vector3 _at;
			double _near;
			double _far;

	public:
		Camera(double dnear, double dfar);
		Camera();
		virtual ~Camera();
		
		void update(float posX, float posY, float posZ, float eyeX, float eyeY, float eyeZ, float upX, float upY, float upZ) {};
		virtual void computeProjectionMatrix() = 0;
		virtual void computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ) = 0;
};





#endif /* __CAMERA_H__ */
