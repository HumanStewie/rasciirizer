#include "../include/Math.h"

#include <cmath>

Vector::Vector3D<float> addVec3(const Vector::Vector3D<float>& v1,
                                const Vector::Vector3D<float>& v2)
{
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.y };
}

Vector::Vector3D<float> subtrVec3(const Vector::Vector3D<float>& v1,
                                  const Vector::Vector3D<float>& v2)
{
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.y };
}

Vector::Vector3D<float> multiVec3(const Vector::Vector3D<float>& v1,
                                  float scalar)
{
    return { v1.x * scalar, v1.y * scalar, v1.z * scalar };
}

float dotVec3(const Vector::Vector3D<float>& v1,
              const Vector::Vector3D<float>& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector::Vector3D<float> crossVec3(const Vector::Vector3D<float>& v1,
                                  const Vector::Vector3D<float>& v2)
{
    return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
             v1.x * v2.y - v1.y * v2.x };
}

float lengthVec3(const Vector::Vector3D<float>& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector::Vector3D<float> normalizeVec3(const Vector::Vector3D<float>& v)
{
    float length { lengthVec3(v) };

    return { v.x / length, v.y / length, v.z / length };
}
