#include <unistd.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <thread>

#include "../include/Math.h"
#include "../include/Matrix.h"
#include "../include/Renderer.h"
#include "../include/Vector2D.h"
#include "../include/Vector3D.h"
#include "../include/Vector4D.h"

/** TODO:
 * - Handle CLIPPING - Partially done, just have a if-else
 * - Work on luminence
 * - Fix FPS problems
 * - Make modular, as in pass any std::vector<Vector3D> Vertex Buffer into our
 * renderer and it will work
 * - Make cube
 */

void Renderer::draw()
{
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        std::cout << (i % m_width ? m_fb[i] : '\n');
    }
}

void Renderer::framebuffer(double A, double B)
{
    const double theta_spacing { 0.07 };
    const double phi_spacing { 0.03 };

    const double cosA { cos(A) };
    const double sinA { sin(A) };
    const double cosB { cos(B) };
    const double sinB { sin(B) };

    Matrix rotateX { 3, 3 };
    rotateX.setIndex(0) = 1;
    rotateX.setIndex(4) = cosA;
    rotateX.setIndex(5) = sinA;
    rotateX.setIndex(7) = -sinA;
    rotateX.setIndex(8) = cosA;
    Matrix rotateZ { 3, 3 };
    rotateZ.setIndex(0) = cosB;
    rotateZ.setIndex(1) = sinB;
    rotateZ.setIndex(3) = -sinB;
    rotateZ.setIndex(4) = cosB;
    rotateZ.setIndex(8) = 1;

    Matrix rotateXZ { rotateX.mulMatrix33(rotateZ.getMat()) };

    const double R1 { 1 };
    const double R2 { 2 };
    const double K2 { 6 };  // How far we want to push object back

    // Uses the calculation from Donut in C to ensure 3/8th screen coverage
    // (75%)
    const double K1 { (K2 * m_width * 1) / (13 * (R1 + R2)) };

    // These loops draw to a local space
    for (double theta {}; theta < 2 * PI; theta += theta_spacing) {
        const double cosTheta { cos(theta) };
        const double sinTheta { sin(theta) };
        for (double phi {}; phi < 2 * PI; phi += phi_spacing) {
            const double cosPhi { cos(phi) };
            const double sinPhi { sin(phi) };

            const double circleX { R2 + R1 * cosTheta };
            const double circleY { R1 * sinTheta };
            // Calculate donut rotation
            const Vector3D rotateTorus {
                circleX * (cosB * cosPhi + sinA * sinB * sinPhi) -
                    circleY * cosA * sinB,
                circleX * (cosB * cosPhi - sinA * cosB * sinPhi) +
                    circleY * cosA * sinB,
                K2 + cosA * circleX * sinPhi + circleY * sinA
            };
            

            // Calculate illumination
            double light { cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi -
                           sinA * sinTheta +
                           cosB *
                               (cosA * sinTheta - cosTheta * sinA * sinPhi) };

            double ooz { 1 / rotateTorus.z };
            int torusXP { static_cast<int>(40 +
                                           2.2 * K1 * rotateTorus.x * ooz) };
            int torusYP { static_cast<int>(10 - K1 * rotateTorus.y * ooz) };

            Vector2D<int> projVector { torusXP, torusYP };

            int output { projVector.x + m_width * projVector.y };
            if (light > 0) {
                if (output < m_length && output >= 0) {
                    if (ooz > m_zb[output] && projVector.x > 0 &&
                        projVector.x < m_width && projVector.y > 0 &&
                        projVector.y < m_height) {
                        m_zb[output] = ooz;
                        int lightIndex {
                            static_cast<int>(light * 8)
                        };  // light goes from -sqrt(2) to sqrt(2), we pick 1
                            // out of 11 char ".,-~:;=!*#$@", so we 8*sqrt(2)
                            // ~= 11.3

                        m_fb[output] = ".,-~:;=!*#$@"[lightIndex];
                    }
                }
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

void Renderer::render()
{
    double A {};
    double B {};
    while (true) {
        clear();
        framebuffer(A, B);
        draw();
        A += 0.07;
        B += 0.03;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
    }
}