#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdio.h>

class Vector3 {
	protected:
		double _x;
		double _y;
		double _z;
	public:
		Vector3();
		Vector3(double x, double y, double z);
		~Vector3();
		double getX() const;
		double getY() const;
		double getZ() const;
		void setX(double x);
		void setY(double x);
		void setZ(double x);
		
		void set(double x, double y, double z);
		Vector3 operator=(const Vector3 &vec);
		Vector3 operator*(double num);
		Vector3 operator+(const Vector3 &vec);
		Vector3 operator-(const Vector3 &vec);
		void print();
};

#endif /* __VECTOR3_H__ */