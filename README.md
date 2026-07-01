# Software Renderer on the Terminal

Inspired by the great Andy Sloane (a1k0n) - Donut in C - rasciirizer is a is a generalized
software "renderer" (more correctly rasterizer) for ASCII art given vertices and faces.

## Basic usage

`#include` or write your own vertices and face data in `src/main.cpp` and run:

```bash
# I recommend Ninja, remove for default generator
cmake -B build -G Ninja # Debug version
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release # Release version

# Debug version
cmake --build build (--clean-first) # Run with --clean-first if you want to clean up build files

# Release version
cmake --build build --config Release

./build/rasciirizer
```

You can change output settings by changing the first parameter of `render()`, resolution can be changed in `<width, height>` template. It had to be templated so I can use `std::array` for framebuffer and zbuffer. More work will be made on making that more customizable and modular.

Additional examples, with cool shapes like penger, pretzel and Miku(!)
can be seen in `examples/` folder. All you need to do is `#include`
them and use their `vs` and `fs` for the renderer! **Fair warning:** Some models have high vertices count, compilation will take quite some time.

## Implementation

The underlying principle is similar to donut in C, in that it go through 3 simple step to render: Rotation -> Translation -> Output. Unlike donut in C, where Andy used mathematical algorithms to generate pixels for the donut, I simply loop through a list of vertices and faces data then rotate, translate and draw the correct pixel at the correct spot. This has the added benefit that you can now output *any* shape, given you have the vertices and faces data of it.

~~For now, it is a simple wireframe renderer for the terminal, and in fact, you probably won't even see the wireframe due to how small terminals "pixels" are. It should be visible if you zoom in, though.~~

Everything should look MUCH better now, proper DDA algorithm is used. In the latest update, though, we have triangle rasterization! No lighting and it is a bit buggy, still being worked on.

To get into the math a bit, to rotate the object, we rotate every "pixels" every frame, which is surprisingly efficient (more in Optimization part):

```cpp
sgm::Vec3D rotatedVec {currentVertex * rotationMatrix};
```

~~`mulMatrixVector3D()` is defined in `Matrix.h` header library (which admittedly need to be improved, and it will in the next iteration).~~

Math library now revamped to be called `sgm`, might make a dedicated repo for it, but for now it supports basic vector matrix multiplication and some defined structs.

After rotating within Local Space, we translate it back away from the camera (which indirectly move it from Local Space to World Space to Camera Space):

```cpp
rotatedVec.z += depth;
```

Finally, we project this using a simple perspective equation and throw it into a framebuffer for printing (which is a vector):

```cpp
sgm::Vec<int, 2> proj {rotatedVec.x / rotatedVec.z, rotatedVec.y / rotatedVec.z};
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

- ~~Draw triangles and fill in those triangles~~ efficiently (isn't efficient, probably)
- ~~With given triangles, find normals and simulate lighting~~
- More optimizations, such as move semantics.
- ~~Fix scaling issue~~
- Make renderer more modular
- ~~Projection scaling issues~~

