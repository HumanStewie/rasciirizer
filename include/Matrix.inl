#pragma once
#include "Matrix.h"

namespace sgm {
    template <typename T, std::size_t row, std::size_t col>
    T Matrix<T, row, col>::operator [](const std::size_t rowI, const std::size_t colI) const
    {
        if (rowI >= m_mat.size() || colI >= m_mat.size())
            throw "Index is not valid.\n";
        return m_mat.at(rowI + m_colSize * colI);  // Column major
    }
}  // namespace sgm
