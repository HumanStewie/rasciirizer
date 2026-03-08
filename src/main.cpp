#include "../include/Renderer.h"

int main()
{
    Renderer renderer { 80, 20, 30 };

    renderer.render({ { 1, 1, 1 },
                      { 1, -1, 1 },
                      { -1, -1, 1 },
                      { -1, 1, 1 },
                      { 1, 1, -1 },
                      { 1, -1, -1 },
                      { -1, -1, -1 },
                      { -1, 1, -1 } });

    return 0;
}