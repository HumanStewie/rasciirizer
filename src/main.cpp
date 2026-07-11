#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <vector>

#include "AssetImporter.h"
#include "Renderer.h"
#include "Window.h"

constexpr int WINDOW_HEIGHT { 720 };
constexpr int WINDOW_WIDTH { 1280 };
constexpr int CHAR_HEIGHT { 16 };
constexpr int CHAR_WIDTH { 8 };
constexpr int FRAME_DURATION { 33 };  // in ms
constexpr float DEPTH { 1.8f };

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    std::vector<sgm::Vec3D> vs {};
    std::vector<sgm::Vec<int, 3>> fs {};
    AssetImporter importer { vs, fs };
    importer.parseObj(argv[1]);

    const Window gameWindow { WINDOW_WIDTH, WINDOW_HEIGHT };
    bool running { true };
    SDL_Event event;

    Renderer<WINDOW_WIDTH / CHAR_WIDTH, WINDOW_HEIGHT / CHAR_HEIGHT> renderer {
        DEPTH, gameWindow.getRawWindow(), WINDOW_WIDTH, WINDOW_HEIGHT
    };

    while (running) {
        const Uint32 tick { static_cast<Uint32>(SDL_GetTicks()) };
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                (event.type == SDL_EVENT_KEY_DOWN &&
                 event.key.key == SDLK_ESCAPE)) {
                running = false;
            }
        }
        renderer.render(vs, fs);
        const Uint32 tickAfter { static_cast<Uint32>(SDL_GetTicks()) };
        const Uint32 tickPassed { tickAfter - tick };
        const Uint32 amountToWait { FRAME_DURATION - tickPassed };
        if (amountToWait > 0) {
            SDL_Delay(amountToWait);
        }
    }

    SDL_Quit();
    return 0;
}
