#include "PointLightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <list>

PointLightSource::PointLightSource(int num) {
	_num = num;
}

PointLightSource::~PointLightSource() {
}

void PointLightSource::data() {

}