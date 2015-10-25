#include "DirectionalLightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <list>

DirectionalLightSource::DirectionalLightSource(int number) {
	_num = number;
}

DirectionalLightSource::~DirectionalLightSource() {
}

void DirectionalLightSource::data() {

}