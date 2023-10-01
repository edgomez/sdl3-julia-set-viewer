// SPDX-License-Identifier: MIT
// Copyright(C) 2023 Edouard Gomez

#include <complex>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>

#include <SDL3/SDL.h>

#include "scope_exit.h"

namespace
{

constexpr int s_window_default_witdh = 640;
constexpr int s_window_default_height = 480;

template <typename T> using unique_ptr_custom_delete = std::unique_ptr<T, std::function<void(T*)>>;

using complex = std::complex<float>;

static constexpr complex julia_c{0.355534f, -0.337292f};
static constexpr int s_iterations = 64;
static constexpr float s_arbitrary_big_norm = 1000.f;
} // namespace

extern "C" int
main(int /* argc */, char** /* argv */)
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

    int w;
    int h;
    int r = SDL_GetRenderOutputSize(renderer, &w, &h);
    if (r)
    {
        std::printf("error: failed retrieving renderer output size\n");
        return EXIT_FAILURE;
    }

    SDL_Texture* texture = nullptr;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET | SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!texture)
    {
        std::printf("error: failed creating the render texture\n");
        return EXIT_FAILURE;
    }

    unique_ptr_custom_delete<unsigned char> buffer{
        static_cast<unsigned char*>(std::malloc(w * h * 4)),
        [](unsigned char* ptr) { std::free(ptr); }};
    if (!buffer)
    {
        std::printf("error: failed creating the render texture buffer\n");
        return EXIT_FAILURE;
    }

    bool quit = false;
    float scale = 1.f;
    complex pos{0.f, 0.f};
    complex displacement{5.f / float(w), 5.f / float(h)};
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
            case SDL_EVENT_KEY_DOWN:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_PAGEUP:
                    scale *= 1.05f;
                    break;
                case SDL_SCANCODE_PAGEDOWN:
                    scale *= 0.95f;
                    break;
                case SDL_SCANCODE_UP:
                    pos += complex{0.f, -scale * displacement.imag()};
                    break;
                case SDL_SCANCODE_DOWN:
                    pos += complex{0.f, scale * displacement.imag()};
                    break;
                case SDL_SCANCODE_LEFT:
                    pos += complex{-scale * displacement.real(), 0.f};
                    break;
                case SDL_SCANCODE_RIGHT:
                    pos += complex{scale * displacement.real(), 0.f};
                    break;
                default:
                    // do nothing
                    break;
                }
                break;
            }
        }

        for (size_t y = 0; y < static_cast<size_t>(h); ++y)
        {
            for (size_t x = 0; x < static_cast<size_t>(w); ++x)
            {
                complex z{scale * (float(x) - float(w) / 2.f) / float(w),
                          scale * (float(y) - float(h) / 2.f) / float(h)};
                int i;

                z += pos;
                for (i = 0; i < s_iterations; ++i)
                {
                    z = z * z + julia_c;
                    if (std::norm(z) > s_arbitrary_big_norm)
                    {
                        break;
                    }
                }

                float lumaf = float(i) / float(s_iterations);
                // lumaf = std::pow(lumaf, 1.f / 2.4f);
                unsigned char luma = static_cast<unsigned char>(lumaf * 255.f);

                buffer.get()[y * (w * 4) + (x * 4) + 0] = 255;
                buffer.get()[y * (w * 4) + (x * 4) + 1] = luma;
                buffer.get()[y * (w * 4) + (x * 4) + 2] = luma;
                buffer.get()[y * (w * 4) + (x * 4) + 3] = luma;
            }
        }

        SDL_Rect rect{0, 0, w, h};
        SDL_UpdateTexture(texture, &rect, buffer.get(), w * 4);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}