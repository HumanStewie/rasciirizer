#include "../examples/penger.h"
#include "../include/Renderer.h"

int main()
{
    Renderer<400, 165> renderer { 2, 30};
    renderer.render(vs, fs);

    return 0;
}
