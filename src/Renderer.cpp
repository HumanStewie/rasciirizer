#include "../include/Renderer.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/Math.h"
#include "../include/Matrix.h"
#include "../include/Vector.h"

/** TODO:
 * - Still have optimization available, ill get to that after my work
 */

void Renderer::draw() const
{
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        // you could also just use 2 loops
        std::cout << (i % static_cast<std::size_t>(m_width) ? m_fb[i] : '\n');
    }
}

void Renderer::drawLine(const Vector::Vector3D<float>& point1,
                        const Vector::Vector3D<float>& point2,
                        std::vector<Vector::Vector3D<float>>& vertices)
{
    // 3D DDA Algorithm
    Vector::Vector3D<float> vec { point2.x - point1.x, point2.y - point1.y,
                                  point2.z - point1.z };

    std::vector<Vector::Vector3D<float>> line {};
    for (float step {}; step < lengthVec3(normalizeVec3(vec)); step += 0.05f) {
        Vector::Vector3D<float> pointAtStep { point1.x + vec.x * step,
                                              point1.y + vec.y * step,
                                              point1.z + vec.z * step };
        line.push_back(pointAtStep);
    }
    // Why????
    for (const auto& e : line) {
        vertices.push_back(e);
    }
}
void Renderer::drawLine(const Vector::Vector2D<int>& point1,
                        const Vector::Vector2D<int>& point2,
                        std::vector<Vector::Vector2D<int>>& vertices)
{
    // 3D DDA Algorithm
    Vector::Vector3D<int> vec { point2.x - point1.x, point2.y - point1.y };

    std::vector<Vector::Vector2D<int>> line {};
    for (float step {}; step < lengthVec3(normalizeVec3(vec)); step += 0.05f) {
        Vector::Vector2D<float> pointAtStep { point1.x + vec.x * step,
                                              point1.y + vec.y * step };
        line.push_back(int);
    }
    // Why????
    for (const auto& e : line) {
        vertices.push_back(e);
    }
}

void Renderer::framebuffer(float A, float B, float C,
                           const std::vector<Vector::Vector3D<float>>& vertices,
                           int vertSize)
{
    // precompute sin cos of 3 dimensions
    const float cosA { static_cast<float>(cos(A)) };
    const float sinA { static_cast<float>(sin(A)) };
    const float cosB { static_cast<float>(cos(B)) };
    const float sinB { static_cast<float>(sin(B)) };
    const float cosC { static_cast<float>(cos(C)) };
    const float sinC { static_cast<float>(sin(C)) };
    const float cosAsinB { cosA * sinB };
    const float sinAsinB { sinA * sinB };

    Matrix rotationMatrix { { cosA * cosB, cosAsinB * sinC - sinA * cosC,
                              cosAsinB * cosC + sinA * sinC },
                            { sinA * cosB, sinAsinB * sinC + cosA * cosC,
                              sinAsinB * cosC - cosA * sinC },
                            { -sinB, cosB * sinC, cosB * cosC } };


    // Uses the calculation from Donut in C to ensure 3/8th screen coverage
    // This is also called "focal length", which i didn't figured out how to
    // make it work. So i just made a small patch to it and pray it runs (line
    // 92) const float objectWidth { 1 }; const float K1 { ((m_depth + 1) *
    // m_width * 1) / (15 * objectWidth) };

    std::vector<Vector::Vector2D<int>> projectedVertices { };
    int count {};
    for (const auto& vertex : vertices) {
        Vector::Vector3D rotateVertex { rotationMatrix.mulMatrixVector3D(
            vertex) };
        rotateVertex.z += m_depth;

        // slight optimization by only doing 1 division instead of 2 every frame
        float ooz { 1 / rotateVertex.z };
        Vector::Vector2D<int> projVertex {
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
                projectedVertices.push_back(Vector::Vector2D<int>(projVertex.x, projVertex.y));
                m_zb[static_cast<std::size_t>(output)] = ooz;
                m_fb[static_cast<std::size_t>(output)] =
                    "*#"[count++ < vertSize ? 1 : 0];  // separate vertices and
                                                        // edges it don't work
                                                        // very well tho

            }
        }
    }
    for (std::size_t vertex{}; vertex < projectedVertices.size(), ++vertex) {
        drawLine(projectedVertices[vertex], projectedVertices[(vertex + 1) % projectedVertices.size()], projectedVertices);
    }
    // Mapping 2D position to a 1D array, making it projects properly in 2D
    int output { projVertex.x + m_width * projVertex.y };
}

void Renderer::clear()
{
    std::cout << "\x1b[2J\x1b[1;1H";
    std::fill(m_fb.begin(), m_fb.end(), ' ');
    std::fill(m_zb.begin(), m_zb.end(), 0);
}

void Renderer::render(const std::vector<Vector::Vector3D<float>>& vertices,
                      const std::vector<std::vector<int>>& fs)
{
    // populating vertices (wireframe)
    std::vector<Vector::Vector3D<float>> newVertices { vertices };
    for (const auto& face : fs) {
        for (std::size_t point {}; point < face.size(); ++point) {
            drawLine(newVertices[face[point]],
                     newVertices[face[(point + 1) % face.size()]], newVertices);
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
