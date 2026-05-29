//
// Created by stew on 29/5/26.
//
#pragma once
#include "Renderer.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "Math.h"
#include "Matrix.h"
#include "Vector.h"

/** TODO:
 * - Still have optimization available, ill get to that after my work
 */
template<int width, int height>
void Renderer<width, height>::draw() const
{
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        // you could also just use 2 loops
        std::cout << (i % static_cast<std::size_t>(m_width) ? m_fb[i] : '\n');
    }
}

template<int width, int height>
void Renderer<width, height>::drawLine(const sgm::Vec3D& point1, const sgm::Vec3D& point2,
                        std::vector<sgm::Vec3D>& vertices) const
{
    // 3D DDA Algorithm
    const sgm::Vec3D vec { point2.x - point1.x, point2.y - point1.y,
                     point2.z - point1.z };

    std::vector<sgm::Vec3D> line {};
    for (float step {}; step < sgm::length(sgm::normalize(vec));
         step += 0.05f) {

        line.emplace_back(point1.x + vec.x * step,
                             point1.y + vec.y * step,
                             point1.z + vec.z * step);
    }
    for (const auto& vert : line) {
        vertices.push_back(vert);
    }
}

template<int width, int height>
void Renderer<width, height>::line(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB, std::vector<sgm::Vec<int, 2>>& wireframe)
{
    // DDA Algorithm
    float dx {static_cast<float>(pointB.x - pointA.x)};
    float dy {static_cast<float>(pointB.y - pointA.y)};
    int step {};
    if (std::abs(dx) >= std::abs(dy))
        step = static_cast<int>(std::abs(dx));
    else
        step = static_cast<int>(std::abs(dy));
    dx /= static_cast<float>(step);
    dy /= static_cast<float>(step);
    float x {static_cast<float>(pointA.x)};
    float y {static_cast<float>(pointA.y)};
    int i {};
    while (i <= step) {
        x += dx;
        y += dy;
        ++i;
        // draw line
        std::size_t output {static_cast<std::size_t>(std::round(x) + m_width * std::round(y))};
        if (output > 0 && output < m_length && x > 0 &&
            x < m_width && y > 0 &&
            y < m_height) {
            m_fb[output] = '*';
        }
    }
}

template<int width, int height>
void Renderer<width, height>::framebuffer(const float A, const float B, const float C,
                           const std::vector<sgm::Vec3D>& vertices,
                           const int vertSize, const std::vector<std::vector<int>>& faces)
{
    // precompute sin cos of 3 dimensions
    const float cosA { (std::cos(A)) };
    const float sinA { (std::sin(A)) };
    const float cosB { (std::cos(B)) };
    const float sinB { (std::sin(B)) };
    const float cosC { (std::cos(C)) };
    const float sinC { (std::sin(C)) };
    const float cosAsinB { cosA * sinB };
    const float sinAsinB { sinA * sinB };

    const sgm::Mat3D rotationMatrix { cosA * cosB,
                                sinA * cosB,
                                -sinB,
                                cosAsinB * sinC - sinA * cosC,
                                sinAsinB * sinC + cosA * cosC,
                                cosB * sinC,
                                cosAsinB * cosC + sinA * sinC,
                                sinAsinB * cosC - cosA * sinC,
                                cosB * cosC };

    std::vector<sgm::Vec<int, 2>> projectedVertices {};
    for (std::size_t i{}; i < vertices.size(); ++i) {
        sgm::Vec3D rotateVertex { vertices[i] * rotationMatrix };
        rotateVertex.z += m_depth;

        // slight optimization by only doing 1 division instead of 2 every frame
        float ooz { 1 / rotateVertex.z };
        const sgm::Vec projVertex { project(rotateVertex, ooz) };
        projectedVertices.push_back(projVertex);

        // Mapping 2D position to a 1D array, making it projects properly in 2D
        const int output { projVertex.x + m_width * projVertex.y };

        // populating buffer
        if (output > 0 && output < m_length && projVertex.x > 0 &&
            projVertex.x < m_width && projVertex.y > 0 &&
            projVertex.y < m_height) {
            if (ooz > m_zb[static_cast<std::size_t>(output)]) {
                m_zb[static_cast<std::size_t>(output)] = ooz;
                m_fb[static_cast<std::size_t>(output)] = '#';
            }
        }
    }
    for (const auto& face : faces) {
        for (std::size_t point {}; point < face.size(); ++point) {
            line(projectedVertices[face[point]],
                     projectedVertices[face[(point + 1) % face.size()]],
                     projectedVertices);
        }
    }
}

template<int width, int height>
void Renderer<width, height>::clear()
{
    std::cout << "\x1b[2J\x1b[1;1H";
    std::ranges::fill(m_fb, ' ');
    std::ranges::fill(m_zb, 0);
}

template<int width, int height>
void Renderer<width, height>::render(const std::vector<sgm::Vec3D>& vertices,
                                     const std::vector<std::vector<int>>& fs)
{

    float A {};
    float B {};
    float C {};
    while (true) {
        clear();
        framebuffer(A, B, C, vertices, std::ssize(vertices), fs);
        draw();
        A += 0.07f;
        B += 0.03f;
        C += 0.05f;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
    }
}
