#include "../examples/penger.h"
#include "../include/Renderer.h"

int main()
{
    Renderer renderer { 2, 80, 20 };
    renderer.render(vs, fs);

    return 0;
}
