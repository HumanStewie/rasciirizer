#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <ranges>

/**
 * @class Matrix
 * @brief Initialize a matrix with its dimension or with specified elements
 *
 */
namespace sgm {
    template <typename T, std::size_t row, std::size_t col>
    class Matrix {
       private:
        unsigned int m_rowSize {};
        unsigned int m_colSize {};
        std::array<T, row * col> m_mat {};

       public:
        Matrix()
            : m_rowSize { row },
              m_colSize { col }
        {
        }
        explicit Matrix(T value) : Matrix()
        {
            static_assert(
                row == col,
                "Diagonal Matrix must be a square matrix (row == col).");
            for (int i {}; i < row; i++) {
                m_mat.at(i + m_colSize * i) = value;
            }
        }
        Matrix(const std::initializer_list<T> elements) : Matrix()
        {
            std::ranges::copy(elements.begin(), elements.end(), m_mat.begin());
        }

        T operator [](std::size_t rowI, std::size_t colI) const;

        // Vector::Vector4D<float> mulMatrixVector4D(
        //     const Vector::Vector4D<float>& vec)
        // {
        //     assert(m_mat.size() == 16);
        //     return { m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2] * vec.z +
        //                  m_mat[3] * vec.w,
        //              m_mat[4] * vec.x + m_mat[5] * vec.y + m_mat[6] * vec.z +
        //                  m_mat[7] * vec.w,
        //              m_mat[8] * vec.x + m_mat[8] * vec.y + m_mat[10] * vec.z
        //              +
        //                  m_mat[11] * vec.w,
        //              m_mat[12] * vec.x + m_mat[13] * vec.y + m_mat[14] *
        //              vec.z +
        //                  m_mat[15] * vec.w };
        // }

        // Matrix mulMatrix33(const std::vector<float>& other_mat)
        // {
        //     Matrix new_mat { 3, 3 };
        //
        //     new_mat.setIndex(0) = m_mat[0] * other_mat[0] +
        //                           m_mat[1] * other_mat[3] +
        //                           m_mat[2] * other_mat[6];
        //     new_mat.setIndex(1) = m_mat[0] * other_mat[1] +
        //                           m_mat[1] * other_mat[4] +
        //                           m_mat[2] * other_mat[7];
        //     new_mat.setIndex(2) = m_mat[0] * other_mat[2] +
        //                           m_mat[1] * other_mat[5] +
        //                           m_mat[2] * other_mat[8];
        //
        //     new_mat.setIndex(3) = m_mat[3] * other_mat[0] +
        //                           m_mat[4] * other_mat[3] +
        //                           m_mat[5] * other_mat[6];
        //     new_mat.setIndex(4) = m_mat[3] * other_mat[1] +
        //                           m_mat[4] * other_mat[4] +
        //                           m_mat[5] * other_mat[7];
        //     new_mat.setIndex(5) = m_mat[3] * other_mat[2] +
        //                           m_mat[4] * other_mat[5] +
        //                           m_mat[5] * other_mat[8];
        //
        //     new_mat.setIndex(6) = m_mat[6] * other_mat[0] +
        //                           m_mat[7] * other_mat[3] +
        //                           m_mat[8] * other_mat[6];
        //     new_mat.setIndex(7) = m_mat[6] * other_mat[1] +
        //                           m_mat[7] * other_mat[4] +
        //                           m_mat[8] * other_mat[7];
        //     new_mat.setIndex(8) = m_mat[6] * other_mat[2] +
        //                           m_mat[7] * other_mat[5] +
        //                           m_mat[8] * other_mat[8];
        //
        //     return new_mat;
        // }

        float& setIndex(const unsigned int index)
        {
            return m_mat[static_cast<std::size_t>(index)];
        }
        std::array<float, row * col> getMat() { return m_mat; }
    };

    using Mat2D = Matrix<float, 2, 2>;
    using Mat3D = Matrix<float, 3, 3>;
    using Mat4D = Matrix<float, 4, 4>;
}  // namespace sgm

#include "Matrix.inl"
