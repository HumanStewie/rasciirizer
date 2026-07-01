//
// Created by stew on 29/5/26.
//
#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Matrix.h"
#include "Renderer.h"
#include "SDL3/SDL_render.h"
#include "Vector.h"
#include "sgm.h"

/** TODO:
 * - Still have optimization available, ill get to that after my work
 */
template <std::size_t width, std::size_t height>
void Renderer<width, height>::draw() const
{
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        // you could also just use 2 loops
        std::cout << (i % static_cast<std::size_t>(m_width) ? m_fb[i] : '\n');
    }
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::line(const sgm::Vec<int, 2>& pointA,
                                   const sgm::Vec<int, 2>& pointB)
{
    // DDA Algorithm
    float dx { static_cast<float>(pointB.x - pointA.x) };
    float dy { static_cast<float>(pointB.y - pointA.y) };
    int step {};
    if (std::abs(dx) >= std::abs(dy))
        step = static_cast<int>(std::abs(dx));
    else
        step = static_cast<int>(std::abs(dy));
    dx /= static_cast<float>(step);
    dy /= static_cast<float>(step);
    float x { static_cast<float>(pointA.x) };
    float y { static_cast<float>(pointA.y) };
    int i {};
    while (i <= step) {
        std::size_t output { static_cast<std::size_t>(
            std::round(x) + static_cast<float>(m_width) * std::round(y)) };
        if (output > 0 && output < m_length && x > 0 &&
            x < static_cast<float>(m_width) && y > 0 &&
            y < static_cast<float>(m_height)) {
            const sgm::Vec grid {getGridCoordinate(x, y)};
            SDL_RenderDebugText(getRawRenderer(), grid.x, grid.y, "#");
        }
        x += dx;
        y += dy;
        ++i;
        // draw line
    }
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::rasterizeTriangle(const sgm::Vec<int, 2>& pointA,
                                                const sgm::Vec<int, 2>& pointB,
                                                const sgm::Vec<int, 2>& pointC, float luminance)
{
    // Get bounding box
    // TODO: Optimization where we just need to calculate within the triangle
    const int minX { std::min({ pointA.x, pointB.x, pointC.x }) };
    const int minY { std::min({ pointA.y, pointB.y, pointC.y }) };
    const int maxX { std::max({ pointA.x, pointB.x, pointC.x }) };
    const int maxY { std::max({ pointA.y, pointB.y, pointC.y }) };

    for (int row { minY }; row < maxY; ++row) {
        for (int col { minX }; col < maxX; ++col) {
            // Check if it's in the triangle or not
            const int w0 { Renderer::orient2D(pointB, pointC, { col, row }) };
            const int w1 { Renderer::orient2D(pointC, pointA, { col, row }) };
            const int w2 { Renderer::orient2D(pointA, pointB, { col, row }) };
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                const std::size_t output { static_cast<std::size_t>(col +
                                                              row * m_width) };
                if (output > 0 && output < m_length && col > 0 &&
                    col < m_width && row > 0 && row < m_height) {
                    const sgm::Vec grid {getGridCoordinate(col, row)};
                    SDL_RenderDebugTextFormat(getRawRenderer(), static_cast<float>(grid.x), static_cast<float>(grid.y), "%c", ".,-~:;=!*#$@"[luminance > 0 ? static_cast<std::size_t>(luminance) : 0]);
                }
            }
        }
    }
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::framebuffer(
    const std::vector<sgm::Vec3D>& vertices,
    const std::vector<std::vector<int>>& faces)
{
    SDL_SetRenderDrawColor(getRawRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);

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
    std::vector<sgm::Vec3D> rotatedVertices {};
    for (std::size_t i {}; i < vertices.size(); ++i) {
        sgm::Vec3D rotateVertex { vertices[i] * rotationMatrix };
        rotateVertex.z += m_depth;
        rotatedVertices.push_back(rotateVertex);
        // slight optimization by only doing 1 division instead of 2 every frame
        float ooz { 1 / rotateVertex.z };
        const sgm::Vec projVertex { project(rotateVertex, ooz) };
        projectedVertices.push_back(projVertex);

        // Mapping 2D position to a 1D array, making it projects properly in 2D
        const std::size_t output { static_cast<std::size_t>(
            projVertex.x + m_width * projVertex.y) };
        // populating buffer
        if (output > 0 && output < m_length && projVertex.x > 0 &&
            projVertex.x < m_width && projVertex.y > 0 &&
            projVertex.y < m_height) {
            if (ooz > m_zb[output]) {
                m_zb[output] = ooz;
                const sgm::Vec gridVertex{ getGridCoordinate(projVertex.x, projVertex.y)};
                SDL_RenderDebugText(this->getRawRenderer(), static_cast<float>(gridVertex.x), static_cast<float>(gridVertex.y), ".");
            }
        }
    }
    for (const auto& face : faces) {
        // for (std::size_t point {}; point < face.size(); ++point) {
        //     line(projectedVertices[static_cast<std::size_t>(face[point])],
        //          projectedVertices[static_cast<std::size_t>(
        //              face[(point + 1) % face.size()])]);
        // }
        sgm::Vec3D vec1 { rotatedVertices[static_cast<std::size_t>(face[1])].x - rotatedVertices[static_cast<std::size_t>(face[0])].x,
                        rotatedVertices[static_cast<std::size_t>(face[1])].y - rotatedVertices[static_cast<std::size_t>(face[0])].y,
                        rotatedVertices[static_cast<std::size_t>(face[1])].z - rotatedVertices[static_cast<std::size_t>(face[0])].z };
        sgm::Vec3D vec2 { rotatedVertices[static_cast<std::size_t>(face[2])].x - rotatedVertices[static_cast<std::size_t>(face[0])].x,
                        rotatedVertices[static_cast<std::size_t>(face[2])].y - rotatedVertices[static_cast<std::size_t>(face[0])].y,
                        rotatedVertices[static_cast<std::size_t>(face[2])].z - rotatedVertices[static_cast<std::size_t>(face[0])].z };
        sgm::Vec3D normalVector { sgm::normalize(sgm::cross(vec1, vec2)) };
        float luminance { 8 * sgm::dot(normalVector, sgm::normalize(lightPos)) };
        rasterizeTriangle(projectedVertices[static_cast<std::size_t>(face[0])],
                          projectedVertices[static_cast<std::size_t>(face[1])],
                          projectedVertices[static_cast<std::size_t>(face[2])], luminance);
    }
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::clear()
{
    SDL_SetRenderDrawColor(this->getRawRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->getRawRenderer());
    std::ranges::fill(m_zb, 0);
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::render(const std::vector<sgm::Vec3D>& vertices,
                                     const std::vector<std::vector<int>>& fs)
{
    A += 0.07f;
    B += 0.03f;
    C += 0.05f;
    clear();
    framebuffer(vertices, fs);
    SDL_RenderPresent(this->getRawRenderer());
}
