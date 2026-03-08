#pragma once

template <typename D>
struct Vector {
    struct Vector2D {
        double x {};
        double y {};
    };

    struct Vector3D {
        double x {};
        double y {};
        double z {};
    };

    struct Vector4D {
        double x {};
        double y {};
        double z {};
        double w {};
    };

    D add(const D& v1, const D& v2);
    
}; 