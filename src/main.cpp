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
                      { -1, 1, -1 } },
                    { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, {0, 4}, {1, 5}, {2, 6}, {3, 7} });

    return 0;
}