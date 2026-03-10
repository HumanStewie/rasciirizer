#include "../include/Renderer.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/Math.h"
#include "../include/Matrix.h"
#include "../include/Vector2D.h"

/** TODO:
 * - Make modular, as in pass any std::vector<Vector3D> Vertex Buffer into our
 * renderer and it will work
 * - Generalized wireframe renderer
 */

void Renderer::draw()
{
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        std::cout << (i % m_width ? m_fb[i] : '\n');
    }
}

const std::vector<Vector3D> Renderer::drawLine(const Vector3D& point1,
                                               const Vector3D& point2)
{
    Vector3D vec { point2.x - point1.x, point2.y - point1.y,
                   point2.z - point1.z };

    std::vector<Vector3D> line {};
    for (double step {}; step < lengthVec3(normalizeVec3(vec)); step += 0.050) {
        Vector3D pointAtStep { point1.x + vec.x * step, point1.y + vec.y * step,
                               point1.z + vec.z * step };
        line.push_back(pointAtStep);
    }
    return line;
}

void Renderer::framebuffer(double A, double B, double C,
                           const std::vector<Vector3D>& vertices)
{
    // Terrible implementation
    // std::vector<Vector3D> newVertices {vertices};
    // for (std::size_t vertex{}; vertex < newVertices.size() - 1; ++vertex) {
    //     const std::vector<Vector3D> line {drawLine(newVertices[vertex],
    //     newVertices[vertex + 1])}; newVertices.insert(newVertices.end(),
    //     line.begin(), line.end());
    // }
    const double cosA { cos(A) };
    const double sinA { sin(A) };
    const double cosB { cos(B) };
    const double sinB { sin(B) };
    const double cosC { cos(C) };
    const double sinC { sin(C) };

    Matrix rotationMatrix { { cosA * cosB, cosA * sinB * sinC - sinA * cosC,
                              cosA * sinB * cosC + sinA * sinC },
                            { sinA * cosB, sinA * sinB * sinC + cosA * cosC,
                              sinA * sinB * cosC - cosA * sinC },
                            { -sinB, cosB * sinC, cosB * cosC } };

    const double K2 { 5 };  // How far we want to push object back
    // Uses the calculation from Donut in C to ensure 3/8th screen coverage
    // (75%)
    // This is also called "focal length"
    const double objectWidth { 2 };
    const double K1 { (K2 * m_width * 3) / (8 * objectWidth) };

    for (const auto& vertex : vertices) {
        Vector3D rotateVertex { rotationMatrix.mulMatrixVector3D(vertex) };
        rotateVertex.z += K2;

        double ooz { 1 / rotateVertex.z };

        Vector2D<int> projVertex {
            static_cast<int>((m_width / 2) +
                             0.6 * m_width * ooz * rotateVertex.x),
            static_cast<int>((m_height / 2 + m_height * ooz * rotateVertex.y))
        };
        int output { projVertex.x + m_width * projVertex.y };

        if (output > 0 && output < m_length && projVertex.x > 0 &&
            projVertex.x < m_width && projVertex.y > 0 &&
            projVertex.y < m_height) {
            if (ooz > m_zb[output]) {
                m_zb[output] = ooz;
                m_fb[output] = '*';
            }
        }
    }
}

void Renderer::clear()
{
    std::cout << "\x1b[2J\x1b[1;1H";
    std::fill(m_fb.begin(), m_fb.end(), ' ');
    std::fill(m_zb.begin(), m_zb.end(), 0);
}

void Renderer::render(const std::vector<Vector3D>& vertices,
                      const std::vector<std::vector<int>>& fs)
{
    std::vector<Vector3D> newVertices { vertices };
    for (const auto& face : fs) {
        for (std::size_t point {}; point < face.size(); ++point) {
            std::vector<Vector3D> line { drawLine(
                newVertices[face[point]],
                newVertices[face[(point + 1) % face.size()]]) };

            for (const auto& e : line) {
                newVertices.push_back(e);
            }
        }
    }

    

    double A {};
    double B {};
    double C {};
    while (true) {
        clear();
        framebuffer(A, B, C, newVertices);
        draw();
        A += 0.07;
        B += 0.07;
        C += 0.07;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
    }
}