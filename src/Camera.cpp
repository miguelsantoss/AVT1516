#include "Camera.h"

Camera::Camera(double dnear, double dfar) {
	_near=dnear;
	_far=dfar;
}

Camera::Camera() {}

Camera::~Camera() {}


void Camera::computeProjectionMatrix() {}

void Camera::computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, 
										double targetX, double targetY, double targetZ, 
										double upX, double upY, double upZ) {}

