#include "DirectionalLightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <list>

DirectionalLightSource::DirectionalLightSource(GLenum number) {
	_num = number;
}

DirectionalLightSource::~DirectionalLightSource() {
}


void DirectionalLightSource::shine() {
	GLfloat ambient[] = { _ambient->getX(), _ambient->getY(),_ambient->getZ(), _ambient->getW() };
	GLfloat diffuse[] = { _diffuse->getX(),_diffuse->getY(), _diffuse->getZ(), _diffuse->getW() };
	GLfloat specular[] = { _specular->getX(), _specular->getY(), _specular->getZ(), _specular->getW() };
	GLfloat direction[] = { _direction->getX(), _direction->getY(), _direction->getZ(), _direction->getW() };

	glLightfv(_num, GL_AMBIENT, ambient);		//sets light that comes from all directions to "day mode"
	glLightfv(_num, GL_DIFFUSE, diffuse);		//diffuse reflection reflects light in all directions
	glLightfv(_num, GL_SPECULAR, specular);		//sets the specular color (white)
	glLightfv(_num, GL_POSITION, direction);	//the direction
}