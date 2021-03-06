#ifndef __PERSPECTIVECAMERA_H__
#define __PERSPECTIVECAMERA_H__

#include <iostream>
#include "Camera.h"

class PerspectiveCamera: public Camera {
	private:
		float _fov;
		float _aspect;
		float _ratio;
		Vector3 _direction;
	public:
		PerspectiveCamera(float fov, float ratio, float dnear, float dfar);
		~PerspectiveCamera();
		void setDirection(float x, float y, float z) { _direction.set(x, y, z); }
		const Vector3& getDirection() { return _direction; }
		void update(float posX, float posY, float posZ, float eyeX, float eyeY, float eyeZ, float upX, float upY, float upZ);
		void computeProjectionMatrix();
		void computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ);
};
#endif /* __PERSPECTIVECAMERA_H__ */
