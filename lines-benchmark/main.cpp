#include <benchmark/benchmark.h>
#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

// DDA version of draw_line_solid
void draw_line_solid_dda(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor) {
    float x0 = aBegin.x;
    float y0 = aBegin.y;
    float x1 = aEnd.x;
    float y1 = aEnd.y;

    float dx = x1 - x0;
    float dy = y1 - y0;
    float steps = std::max(std::abs(dx), std::abs(dy));
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; ++i) {
        if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
            aSurface.set_pixel_srgb(static_cast<int>(x), static_cast<int>(y), aColor);
        }
        x += xIncrement;
        y += yIncrement;
    }
}

// Benchmark for Bresenham line drawing
void benchmark_bresenham(benchmark::State& state) {
    auto const width = std::uint32_t(state.range(0));
    auto const height = std::uint32_t(state.range(1));

    SurfaceEx surface(width, height);
    surface.clear();

    Vec2f start = {100.0f, 100.0f};
    Vec2f end1 = {static_cast<float>(width - 100), static_cast<float>(height - 100)};
    Vec2f end2 = {static_cast<float>(width), height / 2.0f};
    Vec2f end3 = {width / 2.0f, static_cast<float>(height)};
    Vec2f end4 = {0.0f, 0.0f}; // Diagonal back to the origin
    ColorU8_sRGB color{255, 255, 255};

    for (auto _ : state) {
        draw_line_solid(surface, start, end1, color);
        draw_line_solid(surface, start, end2, color);
        draw_line_solid(surface, start, end3, color);
        draw_line_solid(surface, start, end4, color);
        benchmark::ClobberMemory();
    }
}

// Benchmark for DDA line drawing
void benchmark_dda(benchmark::State& state) {
    auto const width = std::uint32_t(state.range(0));
    auto const height = std::uint32_t(state.range(1));

    SurfaceEx surface(width, height);
    surface.clear();

    Vec2f start = {100.0f, 100.0f};
    Vec2f end1 = {static_cast<float>(width - 100), static_cast<float>(height - 100)};
    Vec2f end2 = {static_cast<float>(width), height / 2.0f};
    Vec2f end3 = {width / 2.0f, static_cast<float>(height)};
    Vec2f end4 = {0.0f, 0.0f}; // Diagonal back to the origin
    ColorU8_sRGB color{255, 255, 255};

    for (auto _ : state) {
        draw_line_solid_dda(surface, start, end1, color);
        draw_line_solid_dda(surface, start, end2, color);
        draw_line_solid_dda(surface, start, end3, color);
        draw_line_solid_dda(surface, start, end4, color);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_bresenham)
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(benchmark_dda)
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
