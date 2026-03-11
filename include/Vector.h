#pragma once

/**
 * @class Vector
 * @brief Modular Vector class with no explicit way to call 2D 3D or 4D
 *
 *
 */
// class Vector {
//    private:
//     // Purposefully not initialized to save memory.
//     float m_x;
//     float m_y;
//     float m_z;
//
//    public:
//     Vector() : m_x {}, m_y {}, m_z {} {}
//     Vector(float x) : m_x { x } {}
//     Vector(float x, float y) : m_x { x }, m_y { y } {}
//     Vector(float x, float y, float z) : m_x { x }, m_y { y }, m_z { z } {}
// };

namespace Vector {
    template <typename T>
    struct Vector2D {
        T x {};
        T y {};
    };

    template <typename T>
    struct Vector3D {
        T x {};
        T y {};
        T z {};
    };

    template <typename T>
    struct Vector4D {
        T x {};
        T y {};
        T z {};
        T w {};
    };
}  // namespace Vector
