#include <cstdio>
#include <cstdlib>
#include <functional>

#include <SDL3/SDL.h>

#include "scope_exit.h"

namespace
{

constexpr int s_window_default_witdh = 640;
constexpr int s_window_default_height = 480;

} // namespace

extern "C" int
main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SCOPE_EXIT([]() { SDL_Quit(); });

    SDL_Window* window = nullptr;
    window = SDL_CreateWindow("Julia fractal", s_window_default_witdh, s_window_default_height, 0);
    if (!window)
    {
        std::printf("error: failed creating window\n");
        return EXIT_FAILURE;
    }
    SCOPE_EXIT([window]() { SDL_DestroyWindow(window); });

    SDL_Renderer* renderer = nullptr;
    renderer =
        SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::printf("error: failed creating renderer\n");
        return EXIT_FAILURE;
    }
    SCOPE_EXIT([renderer]() { SDL_DestroyRenderer(renderer); });

    bool quit = false;
    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}