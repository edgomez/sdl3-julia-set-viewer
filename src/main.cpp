#include <cstdlib>
#include <cstdio>

#include <SDL3/SDL.h>

namespace
{
void ReportSDLVersion()
{
    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    std::printf("julia: using SDL %d.%d\n", sdl_version.major, sdl_version.minor);
}

}

extern "C" int main(int argc, char** argv)
{
    ReportSDLVersion();

    return EXIT_SUCCESS;
}