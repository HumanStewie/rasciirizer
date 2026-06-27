#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Renderer.h"
#include "Window.h"
#include "pretzel.h"

constexpr int WINDOW_HEIGHT = 720;
constexpr int WINDOW_WIDTH = 1280;
constexpr int FRAME_DURATION {33}; // in ms

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return -1;
    }

    const Window gameWindow{WINDOW_WIDTH, WINDOW_HEIGHT};
    bool running {true};
    SDL_Event event;

    Renderer<300, 100> renderer {  2, 30, gameWindow.getRawWindow() };
    SDL_SetRenderScale(renderer.getRawRenderer(), 0.5f, 0.5f);
    while (running)
    {
        const Uint32 tick {static_cast<Uint32>(SDL_GetTicks())};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))
            {
                running = false;
            }
        }
        renderer.render(vs, fs);
        const Uint32 tickAfter {static_cast<Uint32>(SDL_GetTicks())};
        const Uint32 tickPassed {tickAfter - tick};
        const Uint32 amountToWait {FRAME_DURATION - tickPassed};
        if (amountToWait > 0 )
        {
            SDL_Delay(amountToWait);
        }
    }

    SDL_Quit();
    return 0;
}
