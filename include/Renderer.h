#pragma once
#include <cmath>
#include <vector>

#include "Vector.h"

template <int width, int height>
class Renderer {
   private:
    float m_depth {};
    int m_width {};
    int m_height {};
    int m_fps {};
    std::size_t m_length {};
    std::array<float, width*height>  m_fb;
    std::array<float, width*height> m_zb {};
    float PI { 3.1415926f };

   public:
    Renderer(const float depth, const int fps = 30)
        : m_depth { depth },
          m_width { width },
          m_height { height },
          m_fps { fps },
          m_length { static_cast<std::size_t>(m_width * m_height) },
          m_fb(m_length),
          m_zb(m_length)
    {
    }

    int getWindowWidth() const { return m_width; }
    int getWindowHeight() const { return m_height; }

    void draw()const;
    void framebuffer(float A, float B, float C,
                     const std::vector<sgm::Vec3D>& vertices,
                     int vertSize);
    void clear();
    void render(const std::vector<sgm::Vec3D>& vertices,
                const std::vector<std::vector<int>>& fs);
    void drawLine(const sgm::Vec3D& point1,
                  const sgm::Vec3D& point2,
                  std::vector<sgm::Vec3D>& vertices)const;
    void drawLine(const sgm::Vec<int, 2>& point1,
                            const sgm::Vec<int, 2>& point2,
                            std::vector<sgm::Vec<int, 2>>& vertices);
    };
