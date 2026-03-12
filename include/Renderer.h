#pragma once
#include <cmath>
#include <vector>

#include "Vector.h"
class Renderer {
   private:
    float m_depth {};
    int m_width {};
    int m_height {};
    int m_fps {};
    std::size_t m_length {};
    std::vector<char> m_fb {};
    std::vector<float> m_zb {};
    float PI { 3.1415926f };

   public:
    Renderer(float depth, int wWidth = 80, int wHeight = 20, int fps = 30)
        : m_depth { depth },
          m_width { wWidth },
          m_height { wHeight },
          m_fps { fps },
          m_length { static_cast<std::size_t>(m_width * m_height) },
          m_fb(m_length),
          m_zb(m_length)
    {
    }

    int getWindowWidth() { return m_width; }
    int getWindowHeight() { return m_height; }

    void draw();
    void framebuffer(float A, float B, float C,
                     const std::vector<Vector::Vector3D<float>>& vertices,
                     int vertSize);
    void clear();
    void render(const std::vector<Vector::Vector3D<float>>& vertices,
                const std::vector<std::vector<int>>& fs);
    void drawLine(const Vector::Vector3D<float>& point1,
                  const Vector::Vector3D<float>& point2,
                  std::vector<Vector::Vector3D<float>>& vertices);
};
