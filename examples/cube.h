#pragma once

#include <vector>

#include "../include/Vector.h"

// Made by me:3
inline std::vector<sgm::Vec3D> vs { { 1, 1, 1 },    { 1, -1, 1 }, { -1, -1, 1 },
                                    { -1, 1, 1 },   { 1, 1, -1 }, { 1, -1, -1 },
                                    { -1, -1, -1 }, { -1, 1, -1 } };

inline std::vector<std::vector<int>> fs {   { 0, 1, 2 }, { 0, 3, 2 },
                                            { 0, 3, 7 }, { 0, 4, 7 },
                                            { 0, 4, 5 }, { 0, 1, 5 },
                                            { 6, 2, 3 }, { 6, 7, 3 },
                                            { 6, 5, 1 }, { 6, 2, 1 },
                                            { 6, 7, 4 }, { 6, 5, 4 }};


// inline std::vector<std::vector<int>> fs { { 0, 1, 2, 3 }, { 4, 5, 6, 7 },
//                                               { 0, 4 },       { 1, 5 },
//                                               { 2, 6 },       { 3, 7 } };