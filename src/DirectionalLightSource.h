#ifndef DIRECTIONAL_LIGHTSOURCE_H_
#define DIRECTIONAL_LIGHTSOURCE_H_

#include "LightSource.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class DirectionalLightSource : public LightSource {
public:
	DirectionalLightSource(GLenum number);
	~DirectionalLightSource();
	void shine();
};

#endif 