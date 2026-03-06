#pragma once
#include <cmath>
#include <vector>
class Renderer {
   private:
    int m_width {};
    int m_height {};
    int m_length {};
    double m_fps {};
    std::vector<char> m_fb {};
    std::vector<double> m_zb {};
    double theta {};
    double PI { 3.14159265 };

   public:
    Renderer(int wWidth = 80, int wHeight = 20, double fps = 30)
        : m_width { wWidth },
          m_height { wHeight },
          m_fps { fps },
          m_length { m_width * m_height },
          m_fb(m_length),
          m_zb(m_length)
    {
    }

    int getWindowWidth() { return m_width; }
    int getWindowHeight() { return m_height; }

    void draw();
    void framebuffer(double A);
    void clear();
    void render();
};
