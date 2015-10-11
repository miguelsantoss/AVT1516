#ifndef __PERSPECTIVECAMERA_H__
#define __PERSPECTIVECAMERA_H__

#include <iostream>
#include "Camera.h"

class PerspectiveCamera: public Camera {
	private:
		double _fov;
		double _aspect;
		double _ratio;
	public:
		PerspectiveCamera();
		~PerspectiveCamera();
		void update(double fov, double ratio, double aspect, double cnear, double cfar);
		void computeProjectionMatrix();
		void computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ);
};
#endif /* __PERSPECTIVECAMERA_H__ */
