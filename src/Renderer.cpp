#include "../include/Renderer.h"

#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "../include/Vector3D.h"

void Renderer::draw()
{
    // using clock = std::chrono::steady_clock;
    // auto duration { 1 / m_fps };
    // auto start { clock::now() };
    std::cout << "\x1b[2J\x1b[1;1H";
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
    double K1 {};
    for (double i {}; i < m_height; ++i) {
        for (double j {}; j < m_width; ++j) {
            Vector3D vec { j, i, 5 };
            double ooz { 1 / vec.z };

            Vector3D projVec { j * ooz, i * ooz, vec.z - A };

            int rast { static_cast<int>(projVec.x + m_width * projVec.y) };
            if ((vec.y == 10 && vec.x == 15)) m_fb[rast] = '#';
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
    double A { 5 };
    while (true) {
        clear();
        framebuffer(A);
        draw();
        if (A > 0)
            A -= 0.0003;
        else
            A = 0;
    }
}