#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Partially Off-Screen Triangle", "[clip][special]")
{
    Surface surface(320, 240);
    surface.clear();

    draw_triangle_interp(surface,
                         { -50.f, 50.f }, { 150.f, 50.f }, { 50.f, 200.f },
                         { 0.5f, 0.3f, 0.7f },
                         { 0.5f, 0.3f, 0.7f },
                         { 0.5f, 0.3f, 0.7f });

    auto const col = find_most_red_pixel(surface);
    REQUIRE(col.r > 0);
    REQUIRE(col.g > 0);
    REQUIRE(col.b > 0);
}
