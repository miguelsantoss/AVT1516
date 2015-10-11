#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <iostream>
#include "Vector3.h"

class Vector4: public Vector3 {
private:
    double _w;
public:
    Vector4();
    Vector4(double in_x, double in_y, double in_z, double in_w);
    ~Vector4();
    double getW();
    void set(double in_x, double in_y, double in_z, double in_w);
    Vector4 operator=(const Vector4 & in_vec);
    Vector4 operator*(double in_num);
    Vector4 operator+(const Vector4 & in_vec);
    Vector4 operator-(const Vector4 & in_vec);
};
#endif /* __VECTOR4_H__ */