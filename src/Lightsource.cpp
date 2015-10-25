#include "LightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

LightSource::LightSource() {
	_state = false;
}

LightSource::~LightSource() {
}

bool LightSource::getState() {
	return _state;
}

void LightSource::setState(bool state) {
	_state = state;
}

void LightSource::setPosition(Vector4 *position) {
	_position = position;
}

void LightSource::setDirection(Vector4 *direction) {
	_direction = direction;
}

void LightSource::setAmbient(Vector4 *ambient) {
	_ambient = ambient;
}

void LightSource::setDiffuse(Vector4 *diffuse) {
	_diffuse = diffuse;
}

void LightSource::setSpecular(Vector4 *specular) {
	_specular = specular;
}