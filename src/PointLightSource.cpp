#include "PointLightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <list>

PointLightSource::PointLightSource() {
	_num = 0;
}

PointLightSource::~PointLightSource() {
}

void PointLightSource::shine() {
	GLfloat ambient[] = { static_cast<GLfloat>(_ambient->getX()), static_cast<GLfloat>(_ambient->getY()), static_cast<GLfloat>(_ambient->getZ()), static_cast<GLfloat>(_ambient->getW()) };
	GLfloat diffuse[] = { static_cast<GLfloat>(_diffuse->getX()),static_cast<GLfloat>(_diffuse->getY()), static_cast<GLfloat>(_diffuse->getZ()), static_cast<GLfloat>(_diffuse->getW()) };
	GLfloat specular[] = { static_cast<GLfloat>(_specular->getX()),static_cast<GLfloat>(_specular->getY()), static_cast<GLfloat>(_specular->getZ()), static_cast<GLfloat>(_specular->getW()) };
	GLfloat position[] = { static_cast<GLfloat>(_position->getX()),static_cast<GLfloat>(_position->getY()), static_cast<GLfloat>(_position->getZ()), static_cast<GLfloat>(_position->getW()) };
	GLfloat direction[] = { static_cast<GLfloat>(_direction->getX()),static_cast<GLfloat>(_direction->getY()), static_cast<GLfloat>(_direction->getZ()), static_cast<GLfloat>(_direction->getW()) };

	glLightfv(_num, GL_DIFFUSE, diffuse);		// Diffuse reflection reflects light in all directions
	glLightfv(_num, GL_SPECULAR, specular);		// Sets the specular color (white)
	glLightfv(_num, GL_POSITION, position);	// The position from which the ligh is shining
	glLightfv(_num, GL_SPOT_DIRECTION, direction);	// Direction where the light is pointing at
	glLightfv(_num, GL_SPOT_CUTOFF, &_cut_off); // 0~180º  intensity distribution of the light
	glLightfv(_num, GL_SPOT_EXPONENT, &_exponent); // 0~128  maximum spread angle of the light source
}