#include "GameObject.h"

GameObject::GameObject() {}
GameObject::GameObject(float offsetX, float offsetY)
{
	_x = offsetX;
	_z = offsetY;
	_xmin = 0;
	_xmax = 0;
	_zmin = 0;
	_zmax = 0;
}
GameObject::~GameObject() {}

double GameObject::getXmin()
{
	return _xmin;
}

double GameObject::getXmax()
{
	return _xmax;
}

double GameObject::getZmin()
{
	return _zmin;
}

double GameObject::getZmax()
{
	return _zmax;
}

double GameObject::getOffsetX()
{
	return _x;
}
double GameObject::getOffsetZ()
{
	return _z;
}

void GameObject::setXmax(double xmax)
{
	_xmax = xmax;
}
void GameObject::setXmin(double xmin)
{
	_xmin = xmin;
}
void GameObject::setZmax(double zmax)
{
	_zmax = zmax;
}
void GameObject::setZmin(double zmin)
{
	_zmin = zmin;
}
bool GameObject::checkColision(GameObject * obj) {
	return ((_xmax > obj->getXmin()) 
		&& (_xmin < obj->getXmax()) 
		&& (_zmax > obj->getZmin()) 
		&& (_zmin < obj->getZmax()));
}