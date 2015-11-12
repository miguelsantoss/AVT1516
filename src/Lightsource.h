#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "Vector4.h"
#include "GL\glut.h"


class LightSource {

protected:
	Vector4 _ambient;
	Vector4 _diffuse;
	Vector4 _specular;
	Vector4 _position;
	Vector4 _direction;
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
	
	void setCutOff(float cut_off) { _cut_off = cut_off; }
	void setExponent(float exponent) { _exponent = exponent; }
	void setPosition(const Vector4& position);
	void setDirection(const Vector4& direction);
	void setAmbient(const Vector4& ambient);
	void setDiffuse(const Vector4& diffuse);
	void setSpecular(const Vector4& specular);
	void setPosition(float x, float y, float z, float w);
	void setDirection(float x, float y, float z, float w);
	void setAmbient(float x, float y, float z, float w);
	void setDiffuse(float x, float y, float z, float w);
	void setSpecular(float x, float y, float z, float w);

	const Vector4& getPosition() { return _position; }
	const Vector4& getDirection() { return _direction; }
	float getCutOff() { return _cut_off; }
	float getExponent() { return _exponent; }
	const Vector4& getAmbient() { return _ambient; }
	const Vector4& getDiffuse() { return _diffuse; }
	const Vector4& getSpecular() { return _specular; }
};

#endif /* LIGHTSOURCE_H_ */