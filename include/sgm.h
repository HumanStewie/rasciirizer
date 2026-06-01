#pragma once

#include <cmath>

#include "Matrix.h"
#include "Vector.h"

// Vector::Vector3D<float> addVec3(const Vector::Vector3D<float>& v1,
//                                 const Vector::Vector3D<float>& v2);
// Vector::Vector3D<float> subtrVec3(const Vector::Vector3D<float>& v1,
//                                   const Vector::Vector3D<float>& v2);
// Vector::Vector3D<float> multiVec3(const Vector::Vector3D<float>& v1,
//                                   float scalar);
// float dotVec3(const Vector::Vector3D<float>& v1,
//               const Vector::Vector3D<float>& v2);
// Vector::Vector3D<float> crossVec3(const Vector::Vector3D<float>& v1,
//                                   const Vector::Vector3D<float>& v2);
// float lengthVec3(const Vector::Vector3D<float>& v);
// Vector::Vector3D<float> normalizeVec3(const Vector::Vector3D<float>& v);

namespace sgm {
    inline Vec2D operator -(const Vec2D& vec1, const Vec2D& vec2)
    {
        return Vec2D { vec1.x - vec2.x, vec1.y - vec2.y };
    }
    inline Vec3D operator -(const Vec3D& vec1, const Vec3D& vec2)
    {
        return Vec3D { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
    }
    inline Vec4D operator -(const Vec4D& vec1, const Vec4D& vec2)
    {
        return Vec4D { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z,
                       vec1.w - vec2.w };
    }
    inline Vec2D operator +(const Vec2D& vec1, const Vec2D& vec2)
    {
        return Vec2D { vec1.x + vec2.x, vec1.y + vec2.y };
    }
    inline Vec3D operator +(const Vec3D& vec1, const Vec3D& vec2)
    {
        return Vec3D { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
    }
    inline Vec4D operator +(const Vec4D& vec1, const Vec4D& vec2)
    {
        return Vec4D { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z,
                       vec1.w + vec2.w };
    }
    inline Vec3D operator *(const Vec3D& vec, const Mat3D& mat)
    {
        return Vec3D {
            mat[0, 0] * vec.x + mat[1, 0] * vec.y + mat[2, 0] * vec.z,
            mat[0, 1] * vec.x + mat[1, 1] * vec.y + mat[2, 1] * vec.z,
            mat[0, 2] * vec.x + mat[1, 2] * vec.y + mat[2, 2] * vec.z
        };
    }
    inline float length(const Vec3D& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
    inline Vec3D normalize(const Vec3D& vec)
    {
        const float vectorLength { length(vec) };

        return { vec.x / vectorLength, vec.y / vectorLength,
                 vec.z / vectorLength };
    }
}  // namespace sgm
