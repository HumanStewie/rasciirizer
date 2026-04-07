#include "../examples/cube.h"
#include "../include/Renderer.h"

int main()
{
    Renderer renderer { 4, 80, 20 };
    renderer.render(vs, fs);

    return 0;
}
