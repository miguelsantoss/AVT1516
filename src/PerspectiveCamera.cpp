#include "PerspectiveCamera.h"
#include "Camera.h"
#include <stdio.h>


PerspectiveCamera::PerspectiveCamera() {}

PerspectiveCamera::~PerspectiveCamera() {}


void PerspectiveCamera::update(double fov, double ratio, double aspect, double dnear, double dfar) {
	_fov = fov;
	_ratio = ratio;
	_aspect = aspect;
	_near = dnear ;
	_far  = dfar ;
}
void PerspectiveCamera::computeProjectionMatrix() {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_fov, _aspect, _near, _far);*/
}

void PerspectiveCamera::computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ) {
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);*/
}
