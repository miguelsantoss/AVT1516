#ifndef __ORTHOGONALCAMERA_H__
#define __ORTHOGONALCAMERA_H__

#include "Camera.h"

class OrthogonalCamera: public Camera {
	
	private:
		double _right;
		double _left;
		double _top;
		double _bottom;
		double _ratio; 
		double _aspect;
	public:
		OrthogonalCamera();
		
		~OrthogonalCamera();
		void update(double right, double left, double bottom, double top, double dnear, double dfar, double ratio, double aspect);
		void computeProjectionMatrix();
		void computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ);
};
#endif /* __ORTHOGONALCAMERA_H__ */
