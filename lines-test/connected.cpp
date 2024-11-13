#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "No gaps", "[connect]" )
{
	Surface surface( 200, 200 );
	surface.clear();

	// A few simple sample cases. Not exhaustive.

	SECTION( "horizontal" )
	{
		draw_line_solid( surface,
			{ 10.f, 170.f },
			{ 110.f, 170.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );

		// There are exactly two pixels with one neighbour: one at each end of
		// the line
		REQUIRE( 2 == counts[1] );

		// There should be a non-zero number of pixels with two neighbours, as
		// the line is longer than 2 pixels.
		REQUIRE( counts[2] > 0 );

		// No pixels should have zero neighbours (=isolated)
		REQUIRE( 0 == counts[0] );

		// There should be no pixels with more than two neighbours!
		for( std::size_t i = 3; i < counts.size(); ++i )
			REQUIRE( 0 == counts[i]  );
	}
	SECTION( "vertical" )
	{
		draw_line_solid( surface,
			{ 64.f, 28.f },
			{ 64.f, 100.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );
		REQUIRE( 2 == counts[1] );
		REQUIRE( counts[2] > 0 );

		REQUIRE( 0 == counts[0] );

		for( std::size_t i = 3; i < counts.size(); ++i )
			REQUIRE( 0 == counts[i]  );
	}

	SECTION( "diagonal" )
	{
		draw_line_solid( surface,
			{ 10.f, 100.f },
			{ 100.f, 10.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );
		REQUIRE( 2 == counts[1] );
		REQUIRE( counts[2] > 0 );

		REQUIRE( 0 == counts[0] );

		for( std::size_t i = 3; i < counts.size(); ++i )
			REQUIRE( 0 == counts[i]  );
	}

	SECTION( "slanted low" )
	{
		draw_line_solid( surface,
			{ 10.f, 10.f },
			{ 100.f, 10.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );
		REQUIRE( 2 == counts[1] );
		REQUIRE( counts[2] > 0 );

		REQUIRE( 0 == counts[0] );

		for( std::size_t i = 3; i < counts.size(); ++i )
			REQUIRE( 0 == counts[i]  );
	}

	SECTION( "slanted high" )
	{
		draw_line_solid( surface,
			{ 10.f, 10.f },
			{ 10.f, 100.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );
		REQUIRE( 2 == counts[1] );
		REQUIRE( counts[2] > 0 );

		REQUIRE( 0 == counts[0] );

		for( std::size_t i = 3; i < counts.size(); ++i )
			REQUIRE( 0 == counts[i]  );
	}
}

TEST_CASE("Connecting two lines smoothly", "[connect]") {
    Surface surface(100, 100);
    surface.clear();

    // Draw two connected lines
    draw_line_solid(surface, {10.f, 10.f}, {50.f, 10.f}, {255, 0, 255});
    draw_line_solid(surface, {50.f, 10.f}, {90.f, 20.f}, {255, 0, 255});

    auto const counts = count_pixel_neighbours(surface);
    REQUIRE(counts[1] == 2);  // Two end pixels
    REQUIRE(counts[2] > 0);   // Pixels in-between with two connections
}
