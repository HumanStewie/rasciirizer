#include "../include/Renderer.h"

#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "../include/Vector3D.h"

/** TODO:
 * - Handle CLIPPING - Partially done, just have a if-else
 * - Work on luminence
 * - Fix FPS problems
 * - Make modular, as in pass any std::vector<Vector3D> Vertex Buffer into our
 * renderer and it will work
 */

void Renderer::draw()
{
    // using clock = std::chrono::steady_clock;
    // auto duration { 1 / m_fps };
    // auto start { clock::now() };
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        std::cout << (i % m_width ? m_fb[i] : '\n');
    }

    // sleep timer based on FPS, limiting FPS
    // auto end { clock::now() };
    // auto dt { end - start };
    // if (dt.count() < m_fps) {
    //     std::chrono::duration<double, std::milli> dtFPS { m_fps - dt.count()
    //     }; auto dt_duration =
    //         std::chrono::duration_cast<std::chrono::milliseconds>(dtFPS);
    //     std::this_thread::sleep_for(dt_duration);
    // }
    sleep(1 / m_fps);
}

void Renderer::framebuffer(double A, double B)
{
    for (double j {}; j < 8.28; j += 0.07) {
        for (double i {}; i < 8.28; i += 0.02) {
            float c = sin(i);
            float d = cos(j);
            float e = sin(A);
            float f = sin(j);
            float g = cos(A);
            float h = d + 2;
            float D = 1 / (c * h * e + f * g + 5);
            float l = cos(i);
            float m = cos(B);
            float n = sin(B);
            float t = c * h * g - f * e;
            int x   = 40 + 2.2 * D * (l * h * m - t * n);
            int y   = 10 +  D * (l * h * n + t * m);
            int o   = x + 80 * y;
            int N =
                8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
            if (22 > y && y > 0 && x > 0 && 80 > x && D > m_zb[o]) {
                m_zb[o] = D;
                m_fb[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
            }
        }
    }
}

void Renderer::clear()
{
    std::fill(m_fb.begin(), m_fb.end(), ' ');
    std::fill(m_zb.begin(), m_zb.end(), 0);
}

void Renderer::render()
{
    double A {};
    double B {};
    while (true) {
        std::cout << "\x1b[2J\x1b[1;1H";

        clear();
        framebuffer(A, B);
        draw();
        A += 0.007;
        B += 0.003;
    }
}