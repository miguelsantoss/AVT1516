#include "Flare.h"

FlareElement::FlareElement(float r, float g, float b, float distance, float size, int textureIndice) {
	_color = Vector3(r, g, b);
	_distance = distance;
	_size = size;
	_textureIndice = textureIndice;
}

FlareElement::~FlareElement() {}