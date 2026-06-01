#include "Renderer.h"
#include "cube.h"

int main()
{
    Renderer<400, 165> renderer { 5, 30 };
    renderer.render(vs, fs);

    return 0;
}
