# SDL3 Julia Set viewer

## Description

A very simple SDL3 Julia Set Viewer.

It uses the function defined as $f(z) = z^{2} + c, \{z, c\}  \in \mathbb{C}$

Everything is computed on CPU, mono-threaded... really not something worth looking at for performance :-)

## How to build

### Pre-requisites

- CMake >= 3.23
- A C++11 compiler toolchain
- SDL3 library

If your system doesn't include `SDL3`, the repository contains everything required
to use the SDL3 source as a GIT submodule.

```bash
git submodule update --init
```

### COnfiguration and build

Then follow the typical `CMake` based project workflow.

```bash
cmake -B build -S . -G "Ninja Multi-Config"
cmake --build build --config Release --target julia
```

## Working on the code

You're free to work the way you like...

However, if you never made your mind about what you consider the best
editor/IDE, a very minimalistic `.vscode` subdirectory is included to
facilitate the editing/debugging of this code with `VSCode`.

The following `VSCode` extensions might help you edit/build/debug this project:

- Microsoft C/C++ (ms-vscode.cpptools)
- CMake-Tools (ms-vscode.cmake-tools)
- CMake Language Support (josetr.cmake-language-support-vscode)
- markdownlint (davidanson.vscode-markdownlint)

`.clang-format` files are provided for automatic indentation. This requires
a `clang-format` tool >= 16.0.

## License

Licensed under MIT. For details see [LICENSE.md](./LICENSE.md) file.
