#pragma once
#include "Matrix.h"

namespace sgm {
    template <typename T, int row, int col>
    T Matrix<T, row, col>::operator [](const int rowI, const int colI) const
    {
        return m_mat.at(rowI + m_colSize * colI);  // Column major
    }
}  // namespace sgm
