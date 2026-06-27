#pragma once
#include <SDL3/SDL.h>
#include <memory>

class Window
{
private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window_;
public:
    Window(const int width, const int height) :
    sdl_window_{SDL_CreateWindow("rasciirizer", width, height, 0), &SDL_DestroyWindow}
    {
    }

    Window& operator=(const Window& other) = delete;
    Window(const Window& other) = delete;

    [[nodiscard]] SDL_Window* getRawWindow() const
    {
        return sdl_window_.get();
    }
};