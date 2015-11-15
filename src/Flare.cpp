#include "Flare.h"

FlareElement::FlareElement(float r, float g, float b, float a, float distance, float size, int textureIndice) {
	_color = Vector4(r, g, b, a);
	_distance = distance;
	_size = size;
	_textureIndice = textureIndice;
}

FlareElement::~FlareElement() {}