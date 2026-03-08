#pragma once
#include <cassert>
#include <vector>

#include "Vector3D.h"
#include "Vector4D.h"

class Matrix {
   private:
    unsigned int m_rowSize {};
    unsigned int m_colSize {};
    std::vector<double> m_mat {};

   public:
    Matrix(unsigned int dimensionX, unsigned int dimensionY)
        : m_mat(dimensionX * dimensionY),
          m_colSize { dimensionY },
          m_rowSize { dimensionY }
    {
    }

    Vector3D mulMatrixVector3D(const Vector3D& vec)
    {
        assert(m_mat.size() == 9);
        return { m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2] * vec.z,
                 m_mat[3] * vec.x + m_mat[4] * vec.y + m_mat[5] * vec.z,
                 m_mat[6] * vec.x + m_mat[7] * vec.y + m_mat[8] * vec.z };
    }

    Vector4D mulMatrixVector4D(const Vector4D& vec)
    {
        assert(m_mat.size() == 16);
        return { m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2] * vec.z +
                     m_mat[3] * vec.w,
                 m_mat[4] * vec.x + m_mat[5] * vec.y + m_mat[6] * vec.z +
                     m_mat[7] * vec.w,
                 m_mat[8] * vec.x + m_mat[8] * vec.y + m_mat[10] * vec.z +
                     m_mat[11] * vec.w,
                 m_mat[12] * vec.x + m_mat[13] * vec.y + m_mat[14] * vec.z +
                     m_mat[15] * vec.w };
    }

    Matrix mulMatrix33(const std::vector<double>& other_mat)
    {
        Matrix new_mat { 3, 3 };

        new_mat.setIndex(0) = m_mat[0] * other_mat[0] +
                              m_mat[1] * other_mat[3] + m_mat[2] * other_mat[6];
        new_mat.setIndex(1) = m_mat[0] * other_mat[1] +
                              m_mat[1] * other_mat[4] + m_mat[2] * other_mat[7];
        new_mat.setIndex(2) = m_mat[0] * other_mat[2] +
                              m_mat[1] * other_mat[5] + m_mat[2] * other_mat[8];

        new_mat.setIndex(3) = m_mat[3] * other_mat[0] +
                              m_mat[4] * other_mat[3] + m_mat[5] * other_mat[6];
        new_mat.setIndex(4) = m_mat[3] * other_mat[1] +
                              m_mat[4] * other_mat[4] + m_mat[5] * other_mat[7];
        new_mat.setIndex(5) = m_mat[3] * other_mat[2] +
                              m_mat[4] * other_mat[5] + m_mat[5] * other_mat[8];

        new_mat.setIndex(6) = m_mat[6] * other_mat[0] +
                              m_mat[7] * other_mat[3] + m_mat[8] * other_mat[6];
        new_mat.setIndex(7) = m_mat[6] * other_mat[1] +
                              m_mat[7] * other_mat[4] + m_mat[8] * other_mat[7];
        new_mat.setIndex(8) = m_mat[6] * other_mat[2] +
                              m_mat[7] * other_mat[5] + m_mat[8] * other_mat[8];

        return new_mat;
    }

    double& setIndex(unsigned int index) { return m_mat[index]; }
    const std::vector<double> getMat() { return m_mat; }
};