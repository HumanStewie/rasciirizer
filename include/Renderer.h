#pragma once
#include <cmath>
#include <vector>

#include "Vector3D.h"
class Renderer {
   private:
    int m_width {};
    int m_height {};
    int m_fps {};
    int m_length {};
    std::vector<char> m_fb {};
    std::vector<double> m_zb {};
    double PI { 3.1415926 };

   public:
    Renderer(int wWidth = 80, int wHeight = 20, int fps = 30)
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
    void framebuffer(double A, double B, double C,
                     const std::vector<Vector3D>& vertices);
    void clear();
    void render(const std::vector<Vector3D>& vertices, const std::vector<std::vector<int>>& fs);
    const std::vector<Vector3D> drawLine(const Vector3D& point1,
                                          const Vector3D& point2);
};
