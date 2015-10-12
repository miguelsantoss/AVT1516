#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdio.h>

class Vector3 {
	public:
		Vector3();
		Vector3(float x, float y, float z);
		~Vector3();
		float getX() const;
		float getY() const;
		float getZ() const;
		void setX(float x);
		void setY(float x);
		void setZ(float x);
		
		void set(float x, float y, float z);
		/*Vector3 operator=(const Vector3 &vec);
		Vector3 operator*(float num);
		Vector3 operator+(const Vector3 &vec);
		Vector3 operator-(const Vector3 &vec);*/
		void print();
	private:
		float _x;
		float _y;
		float _z;
};

#endif /* __VECTOR3_H__ */