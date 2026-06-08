#pragma once
#include <array>
#include <vector>

#include "Vector.h"

template <std::size_t width, std::size_t height>
class Renderer {
   private:
    float m_depth {};
    int m_width {};
    int m_height {};
    int m_fps {};
    std::size_t m_length {};
    std::array<char, width * height> m_fb {};
    std::array<float, width * height> m_zb {};
    float PI { 3.1415926f };
    [[nodiscard]] sgm::Vec<int, 2> project(const sgm::Vec3D& vertex,
                                           const float ooz) const
    {
        // Move to center, then basic projection
        const int projX { static_cast<int>((static_cast<float>((m_width)) / 2) +
                                           static_cast<float>(m_width) * ooz *
                                               vertex.x) };
        const int projY { static_cast<int>(
            (static_cast<float>((m_height)) / 2) -
            static_cast<float>(m_height) * ooz * vertex.y) };

        return { projX, projY };
    }

   public:
    Renderer(const float depth, const int fps = 30)
        : m_depth { depth },
          m_width { width },
          m_height { height },
          m_fps { fps },
          m_length { static_cast<std::size_t>(m_width * m_height) }
    {
    }

    int getWindowWidth() const { return m_width; }
    int getWindowHeight() const { return m_height; }

    void draw() const;
    void framebuffer(float A, float B, float C,
                     const std::vector<sgm::Vec3D>& vertices,
                     const std::vector<std::vector<int>>& faces);
    void clear();
    void render(const std::vector<sgm::Vec3D>& vertices,
                const std::vector<std::vector<int>>& fs);
    void line(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB);
    int orient2D(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB,
                 const sgm::Vec<int, 2>& pointC)
    {
        return (pointB.x - pointA.x) * (pointC.y - pointA.y) -
               (pointC.x - pointA.x) * (pointB.y - pointA.y);
    }
    void rasterizeTriangle(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB, const sgm::Vec<int, 2>& pointC);
};
#include "Renderer.inl"
