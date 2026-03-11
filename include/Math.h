#pragma once

#include "Vector.h"

Vector::Vector3D<float> addVec3(const Vector::Vector3D<float>& v1,
                                const Vector::Vector3D<float>& v2);
Vector::Vector3D<float> subtrVec3(const Vector::Vector3D<float>& v1,
                                  const Vector::Vector3D<float>& v2);
Vector::Vector3D<float> multiVec3(const Vector::Vector3D<float>& v1,
                                  float scalar);
float dotVec3(const Vector::Vector3D<float>& v1,
              const Vector::Vector3D<float>& v2);
Vector::Vector3D<float> crossVec3(const Vector::Vector3D<float>& v1,
                                  const Vector::Vector3D<float>& v2);
float lengthVec3(const Vector::Vector3D<float>& v);
Vector::Vector3D<float> normalizeVec3(const Vector::Vector3D<float>& v);
