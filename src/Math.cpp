#include "../include/Math.h"
#include <cmath>

Vector3D addVec3(const Vector3D& v1, const Vector3D& v2)
{
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.y };
}

Vector3D subtrVec3(const Vector3D& v1, const Vector3D& v2)
{
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.y };
}

Vector3D multiVec3(const Vector3D& v1, double scalar)
{
    return { v1.x * scalar, v1.y * scalar, v1.z * scalar };
}

double dotVec3(const Vector3D& v1, const Vector3D& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D crossVec3(const Vector3D& v1, const Vector3D& v2)
{
    return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
             v1.x * v2.y - v1.y * v2.x };
}

double lengthVec3(const Vector3D& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D normalizeVec3(const Vector3D& v)
{
    double length { lengthVec3(v) };

    return { v.x / length, v.y / length, v.z / length };
}