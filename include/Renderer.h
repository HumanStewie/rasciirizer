#pragma once
#include <array>
#include <vector>

#include "Vector.h"
#include <SDL3/SDL_render.h>
#include <memory>

template <std::size_t width, std::size_t height>
class Renderer {
   public:
    Renderer(const float depth, SDL_Window* window, int winWidth, int winHeight)
        : sdl_renderer_{SDL_CreateRenderer(window, "software"), &SDL_DestroyRenderer},
          m_depth { depth },
          m_wwidth { winWidth },
          m_wheight { winHeight },
          m_width { width },
          m_height { height },
          m_length { static_cast<std::size_t>(m_width * m_height) }
    {
    }
    Renderer(const Renderer& other) = delete;
    Renderer* operator=(const Renderer& other) = delete;

    [[nodiscard]] int getWindowWidth() const { return m_width; }
    [[nodiscard]] int getWindowHeight() const { return m_height; }
    [[nodiscard]] SDL_Renderer* getRawRenderer() const { return sdl_renderer_.get(); }


    void draw() const;
    void framebuffer(const std::vector<sgm::Vec3D>& vertices,
                     const std::vector<std::vector<int>>& faces);
    void clear();
    void render(const std::vector<sgm::Vec3D>& vertices,
                const std::vector<std::vector<int>>& fs);
    void line(const sgm::Vec<int, 2>& pointA, const sgm::Vec<int, 2>& pointB);
    void rasterizeTriangle(const sgm::Vec<int, 2>& pointA,
                           const sgm::Vec<int, 2>& pointB,
                           const sgm::Vec<int, 2>& pointC, float luminance);
private:
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> sdl_renderer_;
    float m_depth {};
    int m_width {};
    int m_height {};
    int m_wwidth {};
    int m_wheight {};
    std::size_t m_length {};
    std::array<char, width * height> m_fb {};
    std::array<float, width * height> m_zb {};

    float A {};
    float B {};
    float C {};
    sgm::Vec3D lightPos { 1.5f, 0, -1.5f };

    [[nodiscard]] sgm::Vec<int, 2> project(const sgm::Vec3D& vertex,
                                           const float ooz) const
    {
        // Move to center, then basic projection
        const int projX { static_cast<int>((static_cast<float>((m_width)) / 2) +
                                           static_cast<float>(m_width) * ooz * vertex.x *
                                               static_cast<float>(m_wheight)/static_cast<float>(m_wwidth)) };
        const int projY { static_cast<int>(
            (static_cast<float>((m_height)) / 2) -
            static_cast<float>(m_height) * ooz * vertex.y) };

        return { projX, projY };
    }
    [[nodiscard]] int orient2D(const sgm::Vec<int, 2>& pointA,
                               const sgm::Vec<int, 2>& pointB,
                               const sgm::Vec<int, 2>& pointC)
    {
        return (pointB.x - pointA.x) * (pointC.y - pointA.y) -
               (pointC.x - pointA.x) * (pointB.y - pointA.y);
    }
    [[nodiscard]] sgm::Vec<int, 2> getGridCoordinate(int x, int y)
    {
        return sgm::Vec<int, 2> {x * 8 + 4, y * 16 + 8};
    }
};
#include "Renderer.inl"
