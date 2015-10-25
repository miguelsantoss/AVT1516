#include "PerspectiveCamera.h"
#include "Camera.h"
#include "GameManager.h"
#include <stdio.h>


PerspectiveCamera::PerspectiveCamera(float fov, float ratio, float dnear, float dfar) {
	_fov = fov;
	_ratio = ratio;
	_near = dnear;
	_far = dfar;
}

PerspectiveCamera::~PerspectiveCamera() {}


void PerspectiveCamera::update(float posX, float posY, float posZ, float eyeX, float eyeY, float eyeZ, float upX, float upY, float upZ) {
	loadIdentity(PROJECTION);
	perspective(53.13f, 1.0f, 0.1f, 1000.0f);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(posX, posY, posZ,
		eyeX, eyeY, eyeZ,
		upX, upY, upZ);
}
void PerspectiveCamera::computeProjectionMatrix() {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_fov, _aspect, _near, _far);*/
	loadIdentity(PROJECTION);
	perspective(_fov, _ratio, _near, _far);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(2, 3, 2,
		10, 0, 10,
		1, 0, 1);
}

void PerspectiveCamera::computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ) {
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);*/
}
