# Software Renderer on the Terminal

Inspired by the great Andy Sloane (a1k0n) - Donut in C - rasciirizer is a is a generalized
software "renderer" (more correctly rasterizer) for ASCII art given vertices and faces.

## Basic usage

`#include` or write your own vertices and face data in `src/main.cpp` and run:

```bash
mkdir build && cd build

cmake ..

cmake --build . (--clean-first) # Run with --clean-first if you want to clean up build files

./rasciirizer
```

You can change output settings by changing the first parameter of `render()`. More work will be made on making that more customizable and modular.

Additional examples, with cool shapes like penger, pretzel and Miku(!)
can be seen in `examples/` folder. All you need to do is `#include`
them and use their `vs` and `fs` for the renderer! **Fair warning:** Some models have high vertices count, compilation will take quite some time.

## Implementation

The underlying principle is similar to donut in C, in that it go through 3 simple step to render: Rotation -> Translation -> Output. Unlike donut in C, where Andy used mathematical algorithms to generate pixels for the donut, I simply loop through a list of vertices and faces data then rotate, translate and draw the correct pixel at the correct spot. This has the added benefit that you can now output *any* shape, given you have the vertices and faces data of it.

For now, it is a simple wireframe renderer for the terminal, and in fact, you probably won't even see the wireframe due to how small terminals "pixels" are. It should be visible if you zoom in, though.

To get into the math a bit, to rotate the object, we rotate every "pixels" every frame, which is surprisingly efficient (more in Optimization part):

```cpp
Vector::Vector3D<float> rotatedVec {rotationMatrix.mulMatrixVector3D(currentVertex)};
```

`mulMatrixVector3D()` is defined in `Matrix.h` header library (which admittedly need to be improved, and it will in the next iteration).

After rotating within Local Space, we translate it back away from the camera (which indirectly move it from Local Space to World Space to Camera Space):

```cpp
rotatedVec.z += depth;
```

Finally, we project this using a simple perspective equation and throw it into a framebuffer for printing (which is a vector):

```cpp
Vector::Vector2D proj {rotatedVec.x / rotatedVec.z, rotatedVec.y / rotatedVec.z};
int output { proj.x + width * proj.y };
framebuffer[output] = '#';
```

That `output` is particularly interesting. That is the equation to convert any point in 2D to 1D buffer, somehow. (more info here: <https://stackoverflow.com/questions/1730961/convert-a-2d-array-index-into-a-1d-index>)

Of course, there are some more additions to the code, particularly in the projection in order to compensate for tall characters in terminal. This part is still being improved.

## Optimization

Various optimization techniques have been used. Notably, precomputing sin and cos,combining matrix multiplication into one operation, reduce the amount of time we need to divide by using `z^-1` and using `float` instead of `double` to save memory.

Earlier, i mentioned computing rotation matrix is fast, this is mainly due to the aforementioned precomputation of sin and cos, ultimately result in pure multiplication and addition, which are very efficient on the CPU.

Surely, there are more, but i have yet to gain the knowledge of C++ itself to use them.

## TODO

- Draw triangles and fill in those triangles efficiently
- With given triangles, find normals and simulate lighting
- More optimizations, such as move semantics.
- Fix scaling issue
- Make renderer more modular
- Projection scaling issues