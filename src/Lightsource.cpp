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

void LightSource::setPosition(const Vector4& position) {
	_position.set(position.getX(), position.getY(), position.getZ(), position.getW());
}

void LightSource::setPosition(float x, float y, float z, float w) {
	_position.set(x, y, z, w);
}

void LightSource::setDirection(const Vector4& direction) {
	_direction.set(direction.getX(), direction.getY(), direction.getZ(), direction.getW());
}

void LightSource::setDirection(float x, float y, float z, float w) {
	_direction.set(x, y, z, w);
}

void LightSource::setAmbient(const Vector4& ambient) {
	_ambient.set(ambient.getX(), ambient.getY(), ambient.getZ(), ambient.getW());
}

void LightSource::setAmbient(float x, float y, float z, float w) {
	_ambient.set(x, y, z, w);
}

void LightSource::setDiffuse(const Vector4& diffuse) {
	_diffuse.set(diffuse.getX(), diffuse.getY(), diffuse.getZ(), diffuse.getW());
}

void LightSource::setDiffuse(float x, float y, float z, float w) {
	_diffuse.set(x, y, z, w);
}

void LightSource::setSpecular(const Vector4& specular) {
	_specular.set(specular.getX(), specular.getY(), specular.getZ(), specular.getW());
}

void LightSource::setSpecular(float x, float y, float z, float w) {
	_specular.set(x, y, z, w);
}