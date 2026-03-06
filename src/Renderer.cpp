#include "../include/Renderer.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "../include/Vector3D.h"

void Renderer::draw()
{
    using clock = std::chrono::steady_clock;
    auto start { clock::now() };
    std::cout << "\x1b[H";
    for (std::size_t i {}; i <= m_length; ++i) {
        // check if current char is the final char in the line or not
        // very clever trick used in Donut in C
        if (i % m_width) {
            std::cout << m_fb[i];
        }
        else {
            std::cout << '\n';
        }
    }

    // sleep timer based on FPS, limiting FPS
    auto end { clock::now() };
    auto dt { end - start };
    std::chrono::duration<double, std::milli> dtFPS { m_fps - dt.count() };
    auto dt_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(dtFPS);
    std::this_thread::sleep_for(dt_duration);
}

void Renderer::framebuffer(float A, float B)
{
    int k {};
    for (float j {}; j < 10.28; j += 0.07) {
        for (float i {}; i < 10.28; i += 0.02) {
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
            int x   = 40 + 30 * D * (l * h * m - t * n);
            int y   = 12 + 15 * D * (l * h * n + t * m);
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
    std::fill(m_fb.begin(), m_fb.end(), '\0');
    std::fill(m_zb.begin(), m_zb.end(), 0);
}

void Renderer::render()
{
    std::cout << "\x1b[2J";
    float A {};
    float B {};
    while (true) {
        framebuffer(A += 0.007, B += 0.003);
        draw();
        clear();
    }
}