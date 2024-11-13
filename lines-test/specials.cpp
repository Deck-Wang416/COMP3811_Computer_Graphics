#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Zero length", "[special]" )
{
	Surface surface( 50, 50 );
	surface.clear();

	// This is a zero-lenght line.
	//
	// The exercise does not specify what should happen in this case. The two
	// sensible options are: 
	//   - draw nothing
	//   - draw a single pixel
	draw_line_solid( surface,
		{ 25.f, 25.f },
		{ 25.f, 25.f },
		{ 255, 255, 255 }
	);

	auto const counts = count_pixel_neighbours( surface );

	// Either: no pixel with zero neighbours (nothing drawn) or one pixel with
	// zero neighbours (single pixel drawn)
	REQUIRE( (0 == counts[0] || 1 == counts[0]) );

	// No other pixels should exist
	for( std::size_t i = 1; i < counts.size(); ++i )
		REQUIRE( 0 == counts[i]  );
}

TEST_CASE("Single pixel line", "[special]") {
    Surface surface(50, 50);
    surface.clear();

    // Draw a single pixel line
    draw_line_solid(surface, {25.f, 25.f}, {25.f, 25.f}, {255, 0, 0});

    auto const counts = count_pixel_neighbours(surface);

    // Ensure that exactly one pixel is drawn
    REQUIRE(1 == counts[0]);
    for (std::size_t i = 1; i < counts.size(); ++i) REQUIRE(0 == counts[i]);
}

TEST_CASE("Small gap line", "[special]") {
    Surface surface(50, 50);
    surface.clear();

    // Two nearly overlapping points, should connect without gaps
    draw_line_solid(surface, {25.f, 25.f}, {26.f, 25.f}, {0, 255, 0});

    auto const counts = count_pixel_neighbours(surface);
    REQUIRE(counts[1] == 2);  // Ensure two connected pixels are drawn
    REQUIRE(counts[0] == 0);  // No isolated pixels
}
