//
// Created by stew on 29/5/26.
//
#pragma once
#include <array>
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
    for (int y {}; y < m_height; y++) {
        for (int x {}; x < m_width; x++) {
            const sgm::Vec grid { getGridCoordinate(x, y) };
            SDL_RenderDebugTextFormat(
                getRawRenderer(), static_cast<float>(grid.x),
                static_cast<float>(grid.y), "%c", m_fb[x + m_width * y]);
        }
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
            const sgm::Vec grid { getGridCoordinate(x, y) };
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
                                                const sgm::Vec<int, 2>& pointC,
                                                float luminance)
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
                const std::size_t output { static_cast<std::size_t>(
                    col + row * m_width) };
                if (output > 0 && output < m_length && col > 0 &&
                    col < m_width && row > 0 && row < m_height) {
                    const sgm::Vec grid { getGridCoordinate(col, row) };
                    SDL_RenderDebugTextFormat(
                        getRawRenderer(), static_cast<float>(grid.x),
                        static_cast<float>(grid.y), "%c",
                        ".,-~:;=!*#$@"[luminance > 0
                                           ? static_cast<std::size_t>(luminance)
                                           : 0]);
                }
            }
        }
    }
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::framebuffer(
    const std::vector<sgm::Vec3D>& vertices,
    const std::vector<sgm::Vec<int, 3>>& faces)
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

    int count {};
    for (const auto& face : faces) {
        sgm::Vec3D vert1 { vertices[static_cast<std::size_t>(face.x)] };
        sgm::Vec3D vert2 { vertices[static_cast<std::size_t>(face.y)] };
        sgm::Vec3D vert3 { vertices[static_cast<std::size_t>(face.z)] };

        // Rotate
        sgm::Vec3D rotVert1 { vert1 * rotationMatrix };
        rotVert1.z += m_depth;
        sgm::Vec3D rotVert2 { vert2 * rotationMatrix };
        rotVert2.z += m_depth;
        sgm::Vec3D rotVert3 { vert3 * rotationMatrix };
        rotVert3.z += m_depth;
        if (rotVert3.z <= 0 || rotVert2.z <= 0 || rotVert1.z <= 0) continue;

        float ooz1 { 1 / rotVert1.z };
        float ooz2 { 1 / rotVert2.z };
        float ooz3 { 1 / rotVert3.z };

        // Project
        sgm::Vec projVert1 { project(rotVert1, ooz1) };
        sgm::Vec projVert2 { project(rotVert2, ooz2) };
        sgm::Vec projVert3 { project(rotVert3, ooz3) };

        std::cout << rotVert1.z << ' ' << rotVert2.z << ' ' << rotVert3.z
                  << '\n';
        // Find face normal
        sgm::Vec3D vec1 { rotVert2.x - rotVert1.x, rotVert2.y - rotVert1.y,
                          rotVert2.z - rotVert1.z };
        sgm::Vec3D vec2 { rotVert3.x - rotVert1.x, rotVert3.y - rotVert1.y,
                          rotVert3.z - rotVert1.z };
        sgm::Vec3D normalVector { sgm::cross(vec1, vec2) };

        if (normalVector.z < 0) {
            count++;
            float luminance { 11 * sgm::dot(sgm::normalize(normalVector),
                                            sgm::normalize(lightPos)) };
            // Get bounding box
            // TODO: Optimization where we just need to calculate within the
            // triangle
            const int minX { std::min(
                { projVert1.x, projVert2.x, projVert3.x }) };
            const int minY { std::min(
                { projVert1.y, projVert2.y, projVert3.y }) };
            const int maxX { std::max(
                { projVert1.x, projVert2.x, projVert3.x }) };
            const int maxY { std::max(
                { projVert1.y, projVert2.y, projVert3.y }) };

            float triArea { static_cast<float>(
                (projVert2.x - projVert1.x) * (projVert3.y - projVert1.y) -
                (projVert3.x - projVert1.x) * (projVert2.y - projVert1.y)) };
            if (triArea == 0) continue;
            for (int row { minY }; row < maxY; ++row) {
                for (int col { minX }; col < maxX; ++col) {
                    if (col >= m_width || col < 0 || row >= m_height || row < 0)
                        continue;

                    // Check if it's in the triangle or not
                    const int w1 { Renderer::orient2D(projVert2, projVert3,
                                                      { col, row }) };
                    const int w2 { Renderer::orient2D(projVert3, projVert1,
                                                      { col, row }) };
                    const int w3 { Renderer::orient2D(projVert1, projVert2,
                                                      { col, row }) };
                    if (w1 < 0 || w2 < 0 || w3 < 0) continue;

                    const std::size_t output { static_cast<std::size_t>(
                        col + row * m_width) };
                    if (output >= m_length || output < 0) continue;

                    // Get Barycentric Coordinate of any point on triangle
                    float lambda1 { w1 / (triArea) };
                    float lambda2 { w2 / (triArea) };
                    float lambda3 { w3 / (triArea) };
                    float ooz { lambda1 * ooz1 + lambda2 * ooz2 +
                                lambda3 * ooz3 };
                    if (ooz <= m_zb[output]) continue;

                    m_zb[output] = ooz;
                    m_fb[output] =
                        ".,-~:;=!*#$@"[luminance > 0
                                           ? static_cast<std::size_t>(luminance)
                                           : 0];
                }
            }
        }
    }
    SDL_RenderDebugTextFormat(this->getRawRenderer(), 10, 700,
                              "Faces count: %d/%d", count, std::ssize(faces));
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::clear()
{
    SDL_SetRenderDrawColor(this->getRawRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->getRawRenderer());
    std::ranges::fill(m_zb, 0);
    std::ranges::fill(m_fb, ' ');
}

template <std::size_t width, std::size_t height>
void Renderer<width, height>::render(const std::vector<sgm::Vec3D>& vertices,
                                     const std::vector<sgm::Vec<int, 3>>& fs)
{
    A += 0.07f;
    B += 0.03f;
    C += 0.05f;
    clear();
    framebuffer(vertices, fs);
    draw();
    SDL_RenderPresent(this->getRawRenderer());
}
