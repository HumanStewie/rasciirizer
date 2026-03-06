#include "../include/Renderer.h"

#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "../include/Vector3D.h"

// TODO: Handle CLIPPING
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

void Renderer::framebuffer(double A)
{
    for (double x { -3 }; x <= 3; x += 0.01) {
        double sinZ { sin(A) };
        double cosZ { cos(A) };
        double y { std::sqrt(9 - std::pow(x, 2)) };
        Vector3D vec { x*cosZ , y, cosZ };
        double ooz { 1 / vec.z };

        Vector3D projV { 40 + 2.2 * vec.x * ooz, 10 + vec.y * ooz, vec.z };
        int out { static_cast<int>(projV.x) +
                  m_width * static_cast<int>(projV.y) };
        if (projV.x > 0 && projV.y > 0 && vec.z > 0.2) {
            m_fb[out]        = '#';
            m_fb[1599 - out] = '#';
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
    while (true) {
        std::cout << "\x1b[2J\x1b[1;1H";

        clear();
        framebuffer(A);
        draw();
        A += 0.04;
    }
}