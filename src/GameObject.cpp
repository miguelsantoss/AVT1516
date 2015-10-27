#include "GameObject.h"

GameObject::GameObject() {}
GameObject::GameObject(double xmin, double xmax, double zmin, double zmax)
{
	_x = xmax - xmin;
	_z = zmax - zmin;
	_xmin = xmin;
	_xmax = xmax;
	_zmin = zmin;
	_zmax = zmax;
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