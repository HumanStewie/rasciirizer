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

    for (float step {}; step < sgm::length(sgm::normalize(vec));
         step += 0.05f) {
        vertices.emplace_back(point1.x + vec.x * step,
                             point1.y + vec.y * step,
                             point1.z + vec.z * step );
    }
}

template<int width, int height>
void Renderer<width, height>::line(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB, std::vector<sgm::Vec<int, 2>>& wireframe)
{

}

template<int width, int height>
void Renderer<width, height>::framebuffer(const float A, const float B, const float C,
                           const std::vector<sgm::Vec3D>& vertices,
                           const int vertSize)
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

    // Uses the calculation from Donut in C to ensure 3/8th screen coverage
    // This is also called "focal length", which i didn't figured out how to
    // make it work. So i just made a small patch to it and pray it runs (line
    // 92) const float objectWidth { 1 }; const float K1 { ((m_depth + 1) *
    // m_width * 1) / (15 * objectWidth) };

    // std::vector<sgm::Vec<int, 2>> projectedVertices {};
    int count {};
    for (const auto& vertex : vertices) {
        sgm::Vec3D rotateVertex { vertex * rotationMatrix };
        rotateVertex.z += m_depth;

        // slight optimization by only doing 1 division instead of 2 every frame
        float ooz { 1 / rotateVertex.z };
        const sgm::Vec<int, 2> projVertex {
            static_cast<int>(
                (static_cast<float>((m_width)) / 2) +
                0.6 * m_width * ooz *
                    rotateVertex.x),  // The small patch in question
            static_cast<int>((static_cast<float>((m_height)) / 2) -
                             m_height * ooz * rotateVertex.y)
        };

        // Mapping 2D position to a 1D array, making it projects properly in 2D
        int output { projVertex.x + m_width * projVertex.y };

        // populating buffer
        if (output > 0 && output < m_length && projVertex.x > 0 &&
            projVertex.x < m_width && projVertex.y > 0 &&
            projVertex.y < m_height) {
            if (ooz > m_zb[static_cast<std::size_t>(output)]) {
                // fill in lines here
                // projectedVertices.push_back(
                //     sgm::Vec<int, 2>(projVertex.x, projVertex.y));
                m_zb[static_cast<std::size_t>(output)] = ooz;
                m_fb[static_cast<std::size_t>(output)] =
                    "*#"[count++ < vertSize ? 1 : 0];  // separate vertices and
                                                       // edges it don't work
                                                       // very well tho
            }
        }
    }
    // for (std::size_t vertex {}; vertex < projectedVertices.size(), ++vertex)
    // {
    //     drawLine(projectedVertices[vertex],
    //              projectedVertices[(vertex + 1) % projectedVertices.size()],
    //              projectedVertices);
    // }
    // // Mapping 2D position to a 1D array, making it projects properly in 2D
    // int output { projVertex.x + m_width * projVertex.y };
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
    // populating vertices (wireframe)
    std::vector<sgm::Vec3D> newVertices { vertices };
    for (const auto& face : fs) {
        for (std::size_t point {}; point < face.size(); ++point) {
            drawLine(newVertices[face[point]],
                     newVertices[face[(point + 1) % face.size()]],
                     newVertices);
        }
    }

    float A {};
    float B {};
    float C {};
    while (true) {
        clear();
        framebuffer(A, B, C, newVertices, std::ssize(vertices));
        draw();
        A += 0.07f;
        B += 0.03f;
        C += 0.05f;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
    }
}
