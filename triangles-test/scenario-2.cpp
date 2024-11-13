#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Small Triangle", "[small][basic]")
{
    Surface surface(320, 240);
    surface.clear();

    draw_triangle_interp(surface,
                         { 150.f, 150.f }, { 152.f, 150.f }, { 151.f, 152.f },
                         { 0.3f, 0.6f, 0.9f },
                         { 0.3f, 0.6f, 0.9f },
                         { 0.3f, 0.6f, 0.9f });

    auto const col = find_most_red_pixel(surface);
    REQUIRE(col.r > 0);
    REQUIRE(col.g > 0);
    REQUIRE(col.b > 0);
}
