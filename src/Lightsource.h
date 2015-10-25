#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "Vector4.h"
#include "GL\glut.h"


class LightSource {

protected:
	Vector4 *_ambient;
	Vector4 *_diffuse;
	Vector4 *_specular;
	Vector4 *_position;
	Vector4 *_direction;
	float _cut_off;
	float _exponent;
	bool _state;
	bool _isLocal;
	bool _isSpot;

public:
	LightSource();
	virtual ~LightSource();

	bool getState();
	bool getIsLocal() { return _isLocal; }
	bool getIsSpot() { return _isSpot; }

	void setState(bool state);
	void setIsLocal(bool isLocal) { _isLocal = isLocal; }
	void setIsSpot(bool isSpot) { _isSpot = isSpot; }

	GLenum getNum();
	void setPosition(Vector4 *position);
	void setDirection(Vector4 *direction);
	void setCutOff(float cut_off) { _cut_off = cut_off; }
	void setExponent(float exponent) { _exponent = exponent; }
	void setAmbient(Vector4 *ambient);
	void setDiffuse(Vector4 *diffuse);
	void setSpecular(Vector4 *specular);

	Vector4* getPosition() { return _position; }
	Vector4* getDirection() { return _direction; }
	float getCutOff() { return _cut_off; }
	float getExponent() { return _exponent; }
	Vector4* getAmbient() { return _ambient; }
	Vector4* getDiffuse() { return _diffuse; }
	Vector4* getSpecular() { return _specular; }
};

#endif /* LIGHTSOURCE_H_ */