#include "../examples/cube.h"
#include "../include/Renderer.h"

int main()
{
    Renderer<200, 55> renderer { 4, 30};
    renderer.render(vs, fs);

    return 0;
}
