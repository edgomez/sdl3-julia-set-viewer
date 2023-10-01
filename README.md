# SDL3 Julia Set viewer

## Description

A very simple SDL3 Julia Set Viewer.

It uses the function defined as $f(z) = z^{2} + c, \{z, c\}  \in \mathbb{C}$

Everything is computed on CPU, mono-threaded... really not something worth looking at for performance :-)

![Screenshot](/doc/julia-set-(0.245534-0.587292i).png?raw=true "Mandatory screenshot")

## Usage

The following keyboard keys can be used to explore the Julia Set being rendered:

- <kbd>Page Up</kbd>: zoom out
- <kbd>Page Down</kbd>: zoom in
- <kbd>Up</kbd>: move up
- <kbd>Down</kbd>: move down
- <kbd>Right</kbd>: move right
- <kbd>Left</kbd>: move left
- <kbd>i</kbd>: decrease the imaginary part of the julia c constant
- <kbd>Shift</kbd>+<kbd>i</kbd>: increase the imaginary part of the julia c constant
- <kbd>r</kbd>: decrease the real part of the julia c constant
- <kbd>Shift</kbd>+<kbd>r</kbd>: increase the real part of the julia c constant
- <kbd>p</kbd>: print the current Julia set constant and rendering position on console

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

### Configuration and build

Then follow the typical `CMake` based project workflow.

```bash
cmake -B build -S . -G "Ninja Multi-Config"
cmake --build build --config Release --target sdl3-julia-set-viewer
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
