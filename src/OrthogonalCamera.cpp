#include "OrthogonalCamera.h"
#include "Camera.h"
#include "GameManager.h"
#include <stdio.h>

OrthogonalCamera::OrthogonalCamera(double right, double left, double bottom, double top, double dnear, double dfar) {
	_right = right;
	_left = left;
	_bottom = bottom;
	_top = top;
	_near = dnear;
	_far = dfar;
}

OrthogonalCamera::~OrthogonalCamera() {}

void OrthogonalCamera::update(double right, double left, double bottom, double top, double dnear, double dfar, double ratio, double aspect) {
	_right = right;
	_left = left;
	_bottom = bottom;
	_top = top;
	_near = dnear; 
	_far = dfar;
}

void OrthogonalCamera::computeProjectionMatrix() {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_right, _left, _bottom, _top, _near, _far);*/
	loadIdentity(PROJECTION);
	ortho(_right, _left, _bottom, _top, _near, _far);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(0, 5, 0,
		0, 0, 0,
		1, 0, 0);
}

void OrthogonalCamera::computeVisualizationMatrix(double eyeX, double eyeY, double eyeZ, double targetX, double targetY, double targetZ, double upX, double upY, double upZ){
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
}