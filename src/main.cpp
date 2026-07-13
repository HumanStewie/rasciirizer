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
constexpr float DEPTH { 5.8f };

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    std::vector<sgm::Vec3D> vs {};
    std::vector<sgm::Vec<int, 3>> fs {};
    AssetImporter importer { vs, fs };

    const Window gameWindow { WINDOW_WIDTH, WINDOW_HEIGHT };
    bool running { true };
    SDL_Event event;

    Renderer<WINDOW_WIDTH / CHAR_WIDTH, WINDOW_HEIGHT / CHAR_HEIGHT> renderer {
        DEPTH, gameWindow.getRawWindow(), WINDOW_WIDTH, WINDOW_HEIGHT
    };
    bool rendering{true};
    while (running) {
        //const Uint32 tick { static_cast<Uint32>(SDL_GetTicks()) };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
                break;
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                rendering = false;
                break;
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                rendering = true;
                break;
            case SDL_EVENT_DROP_FILE:
                importer.parseGltf(event.drop.data);
                break;
            }
        }
        //if (rendering)
        //    renderer.render(vs, fs);
        //const Uint32 tickAfter { static_cast<Uint32>(SDL_GetTicks()) };
        //const Uint32 tickPassed { tickAfter - tick };
        //const Uint32 amountToWait { FRAME_DURATION - tickPassed };
        //if (amountToWait > 0) {
        //    SDL_Delay(amountToWait);
        //}
    }

    SDL_Quit();
    return 0;
}
