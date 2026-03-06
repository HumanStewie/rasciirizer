#include "../include/Renderer.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "../include/Vector3D.h"

void Renderer::draw()
{
    using clock = std::chrono::steady_clock;
    auto start { clock::now() };

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

void Renderer::framebuffer()
{
    Vector3D vec { 1, 1, -4 };
    double ooz { 1 / vec.z };

    int xp { static_cast<int>(vec.x * ooz) };
    int yp { static_cast<int>(vec.y * ooz) };

    m_zb[xp + 80 * yp] = ooz;
    m_fb[xp + 80 * yp] = '#';
}

void Renderer::clear()
{
    
    m_fb.assign(m_length, ' ');

   
    m_zb.assign(m_length, 0);
}

void Renderer::render()
{
    while(true) {
        framebuffer();
        draw();
        clear();
    }
}