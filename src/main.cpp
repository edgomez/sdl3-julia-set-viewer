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

// You can try different constant complex here.
// See https://paulbourke.net/fractals/juliaset/
complex julia_c{0.355534f, -0.337292f};

// Displacement value when key pressing r/R or i/I
static constexpr complex julia_c_displacement{0.001f, 0.001f};

// Number of iterations before stating the norm of the series reaches infinity
static constexpr int s_iterations = 64;

// This is infinity here ;-)
static constexpr float s_arbitrary_big_norm = 1000.f;
} // namespace

extern "C" int
main(int /* argc */, char** /* argv */)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SCOPE_EXIT([]() { SDL_Quit(); });

    SDL_Window* window = nullptr;
    window =
        SDL_CreateWindow("Julia Set viewer", s_window_default_witdh, s_window_default_height, 0);
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

    unique_ptr_custom_delete<unsigned char> color_palette{
        static_cast<unsigned char*>(std::malloc(256 * 3)),
        [](unsigned char* ptr) { std::free(ptr); }};
    if (!color_palette)
    {
        std::printf("error: failed allocating color palette");
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < static_cast<size_t>(256); ++i)
    {
        unsigned char* color = &color_palette.get()[3 * i];
        color[0] = static_cast<unsigned char>(i);
        color[1] = static_cast<unsigned char>(i);
        color[2] = static_cast<unsigned char>(i);
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
                case SDL_SCANCODE_R:
                    if (e.key.keysym.mod & SDL_KMOD_SHIFT)
                        julia_c += complex{julia_c_displacement.real(), 0.f};
                    else
                        julia_c -= complex{julia_c_displacement.real(), 0.f};
                    break;
                case SDL_SCANCODE_I:
                    if (e.key.keysym.mod & SDL_KMOD_SHIFT)
                        julia_c += complex{0.f, julia_c_displacement.imag()};
                    else
                        julia_c -= complex{0.f, julia_c_displacement.imag()};
                    break;
                case SDL_SCANCODE_P:
                    std::printf("c = %f + %fi, pos = %f + %fi\n", julia_c.real(), julia_c.imag(),
                                pos.real(), pos.imag());
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
                size_t luma = static_cast<size_t>(std::max(std::min(lumaf * 255.f, 255.f), 0.f));
                unsigned char const* color = &color_palette.get()[luma * 3];

                buffer.get()[y * (w * 4) + (x * 4) + 0] = 255;
                buffer.get()[y * (w * 4) + (x * 4) + 1] = color[0];
                buffer.get()[y * (w * 4) + (x * 4) + 2] = color[1];
                buffer.get()[y * (w * 4) + (x * 4) + 3] = color[2];
            }
        }

        SDL_Rect rect{0, 0, w, h};
        SDL_UpdateTexture(texture, &rect, buffer.get(), w * 4);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}