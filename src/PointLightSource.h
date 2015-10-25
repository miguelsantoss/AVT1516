
#ifndef POINT_LIGHTSOURCE_H_
#define POINT_LIGHTSOURCE_H_
#include "LightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class PointLightSource : public LightSource {
public:
	PointLightSource(int num);
	~PointLightSource();
	void data();
	const int getNum() const { return _num; }
	const float getConstantAttenuation() const { return _constantAttenuation; }
	const float getLinearAttenuation() const { return _linearAttenuation; }
	const float getQuadraticAttenuation() const { return _quadraticAttenuation; }
	void setConstantAttenuation(float constantAttenuation) { _constantAttenuation = constantAttenuation; }
	void setLinearAttenuation(float linearAttenuation) { _linearAttenuation = linearAttenuation; }
	void setQuadraticAttenuation(float quadraticAttenuation) { _quadraticAttenuation = quadraticAttenuation; }
private:
	int _num;
	float _constantAttenuation;
	float _linearAttenuation;
	float _quadraticAttenuation;
};

#endif 