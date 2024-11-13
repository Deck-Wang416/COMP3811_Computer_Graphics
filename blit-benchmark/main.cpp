#include <benchmark/benchmark.h>

#include <algorithm>
#include <cassert>
#include <cstring>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
    // Default blit with alpha masking
    void default_blit_earth_(benchmark::State& aState)
    {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        SurfaceEx surface(width, height);
        surface.clear();

        auto source = load_image("assets/earth.png");
        assert(source);

        for (auto _ : aState)
        {
            blit_masked(surface, *source, {500.f, 500.f});
            benchmark::ClobberMemory();
        }

        auto const maxBlitX = std::min(width, source->get_width());
        auto const maxBlitY = std::min(height, source->get_height());

        aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
    }

    // Blit without alpha masking using loops
    void blit_no_alpha_loop_(benchmark::State& aState)
    {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        SurfaceEx surface(width, height);
        surface.clear();

        auto source = load_image("assets/earth.png");
        assert(source);

        for (auto _ : aState)
        {
            for (std::uint32_t y = 0; y < source->get_height(); ++y)
            {
                for (std::uint32_t x = 0; x < source->get_width(); ++x)
                {
                    auto color = source->get_pixel(x, y);
                    surface.set_pixel_srgb(x, y, {color.r, color.g, color.b});
                }
            }
            benchmark::ClobberMemory();
        }

        auto const maxBlitX = std::min(width, source->get_width());
        auto const maxBlitY = std::min(height, source->get_height());

        aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
    }

    // Blit without alpha masking using memcpy for each row
    void blit_no_alpha_memcpy_(benchmark::State& aState)
    {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        SurfaceEx surface(width, height);
        surface.clear();

        auto source = load_image("assets/earth.png");
        assert(source);

        auto sourceData = source->get_image_ptr();
        auto surfaceData = surface.get_surface_ptr();
        auto const rowSize = source->get_width() * 4;

        for (auto _ : aState)
        {
            for (std::uint32_t y = 0; y < source->get_height(); ++y)
            {
                std::memcpy(
                    surfaceData + y * width * 4,
                    sourceData + y * rowSize,
                    rowSize
                );
            }
            benchmark::ClobberMemory();
        }

        auto const maxBlitX = std::min(width, source->get_width());
        auto const maxBlitY = std::min(height, source->get_height());

        aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
    }
}

BENCHMARK(default_blit_earth_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(blit_no_alpha_loop_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(blit_no_alpha_memcpy_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
