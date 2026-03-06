#pragma once
#include <vector>
class Renderer {
   private:
    int m_width {};
    int m_height {};
    int m_fps{};

   public:
    Renderer(int wWidth = 1280, int wHeight = 720, int fps = 30)
        : m_width { wWidth }, m_height { wHeight }, m_fps {fps}
    {
    }

    int getWindowWidth() { return m_width; }
    int getWindowHeight() { return m_height; }

    void draw();
    void framebuffer();
    void clear();
    void render();

};
