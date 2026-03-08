#pragma once

#include "Vector3D.h"


Vector3D addVec3(const Vector3D& v1, const Vector3D& v2);

Vector3D subtrVec3(const Vector3D& v1, const Vector3D& v2);

Vector3D multiVec3(const Vector3D& v1, double scalar);

double dotVec3(const Vector3D& v1, const Vector3D& v2);

Vector3D crossVec3(const Vector3D& v1, const Vector3D& v2);

double lengthVec3(const Vector3D& v);

Vector3D normalizeVec3(const Vector3D& v);