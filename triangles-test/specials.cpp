#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Fullscreen", "[special]" )
{
	
	Surface surface( 320, 240 );
	surface.fill( { 255, 255, 255 } );

	draw_triangle_interp( surface,
		{ -5.f, -5.f }, { 700.f, -5.f }, { -5.f, 500.f },
		{ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }
	);

	auto const col = find_most_red_pixel( surface );
	REQUIRE( 0 == int(col.r) );
	REQUIRE( 0 == int(col.g) );
	REQUIRE( 0 == int(col.b) );
}

TEST_CASE("Near Transparent Triangle", "[transparency][special]")
{
    Surface surface(320, 240);
    surface.clear();

    draw_triangle_interp(surface,
                         { 60.f, 60.f }, { 260.f, 60.f }, { 160.f, 210.f },
                         { 0.1f, 0.1f, 0.1f },
                         { 0.1f, 0.1f, 0.1f },
                         { 0.1f, 0.1f, 0.1f });

    auto const col = find_least_red_nonzero_pixel(surface);
    REQUIRE(col.r > 0);
    REQUIRE(col.g > 0);
    REQUIRE(col.b > 0);
}
