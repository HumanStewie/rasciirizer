#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Renderer.h"
#include "cube.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    SDL_SetAppMetadata("rasciirizer", "1.0", "balls");
    Renderer<400, 165> renderer { 6, 30 };
    renderer.render(vs, fs);

    return 0;
}
