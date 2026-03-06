#pragma once
#include <vector>

namespace Matrices {
    struct Matrix3D {
        std::vector<double> mat3 { std::vector<double>(9) };
    };

    struct Matrix2D {
        std::vector<double> mat2 {std::vector<double> (4)};
    };
}  // namespace Matrices