#pragma once

#include <vector>
#include "../include/Vector.h"

// Made by me:3
inline std::vector<Vector::Vector3D<float>> vs {
    { 1, 1, 1 },  { 1, -1, 1 },  { -1, -1, 1 },  { -1, 1, 1 },
    { 1, 1, -1 }, { 1, -1, -1 }, { -1, -1, -1 }, { -1, 1, -1 }
};

inline std::vector<std::vector<int>> fs { { 0, 1, 2, 3 }, { 4, 5, 6, 7 },
                                              { 0, 4 },       { 1, 5 },
                                              { 2, 6 },       { 3, 7 } };