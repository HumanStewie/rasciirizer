#pragma once
struct Vector2D {
    double x {};
    double y {};

    // Vector2Doperations
    Vector2D add(const Vector2D& v1, const Vector2D& v2);
    Vector2D subtr(const Vector2D& v1, const Vector2D& v2);
    Vector2D mult(const Vector2D& v1, double scalar);

    double dot(const Vector2D& v1, const Vector2D& v2);
    Vector2D cross(const Vector2D& v1, const Vector2D& v2);

    double length(const Vector2D& v);

    Vector2D normalize(const Vector2D& v);
};