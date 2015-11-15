#ifndef __FLARE_H__
#define __FLARE_H__

#include "Vector3.h"

class FlareElement {
private:
	float _distance;
	float _size;
	int _textureIndice;
	Vector3 _color;
public:
	void setDistance(float distance) { _distance = distance; }
	void setSize(float size) { _size = size; }
	void setTextureIndice(int textureIndice) { _textureIndice = textureIndice; }
	void setColor(float r, float g, float b) { _color.set(r, g, b); }


	float getDistance() { return _distance; }
	float getSize() { return _size; }
	int getTextureIndice() { return _textureIndice; }
	const Vector3& getColor() { return _color; }

	FlareElement(float r, float g, float b, float distance, float size, int textureIndice);
	~FlareElement();
};
#endif /* __FLARE_H__ */
