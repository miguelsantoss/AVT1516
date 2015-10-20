
#ifndef POINT_LIGHTSOURCE_H_
#define POINT_LIGHTSOURCE_H_
#include "LightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class PointLightSource : public LightSource {
public:
	PointLightSource(GLenum number);
	~PointLightSource();
	void shine();
};

#endif 