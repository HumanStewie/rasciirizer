#include "../examples/cube.h"
#include "../include/Renderer.h"

int main()
{
    Renderer<80, 20> renderer { 4, 30};
    renderer.render(vs, fs);

    return 0;
}
