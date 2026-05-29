#include "../examples/pretzel.h"
#include "../include/Renderer.h"

int main()
{
    Renderer<400, 165> renderer { 15, 30};
    renderer.render(vs, fs);

    return 0;
}
