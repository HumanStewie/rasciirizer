#pragma once

/**
 * @class Vector
 * @brief Modular Vector class with no explicit way to call 2D 3D or 4D
 */
namespace sgm {
    template <typename T, int dimension>
    struct Vec {};

    template <typename T>
    struct Vec<T, 2> {
        T x {};
        T y {};
    };
    template <typename T>
    struct Vec<T, 3> {
        T x {};
        T y {};
        T z {};
    };
    template <typename T>
    struct Vec<T, 4> {
        T x {};
        T y {};
        T z {};
        T w {};
    };

    using Vec2D = Vec<float, 2>;
    using Vec3D = Vec<float, 3>;
    using Vec4D = Vec<float, 4>;
}  // namespace sgm
