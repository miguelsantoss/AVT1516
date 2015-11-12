#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include "Vector3.h"

class Vector4: public Vector3 {
	public:
		Vector4();
		Vector4(float in_x, float in_y, float in_z, float in_w);
		~Vector4();
		float getW() const;
		void set(float in_x, float in_y, float in_z, float in_w);
		float* getArray();
		/*Vector4 operator=(const Vector4 & in_vec);
		Vector4 operator*(float in_num);
		Vector4 operator+(const Vector4 & in_vec);
		Vector4 operator-(const Vector4 & in_vec);*/
	private:
		float _w;
};
#endif /* __VECTOR4_H__ */