#include <benchmark/benchmark.h>

#include <algorithm>
#include <cassert>
#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"
#include <cstring>

namespace
{
	void default_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for( auto _ : aState )
		{
			blit_masked( surface, *source, {500.f, 500.f} );
			benchmark::ClobberMemory(); 
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}

	void simple_blit_no_alpha( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for (auto _ : aState)
		{
			for (std::uint32_t y = 0; y < source->get_height(); ++y)
			{
				for (std::uint32_t x = 0; x < source->get_width(); ++x)
				{
					surface.set_pixel(x, y, source->get_pixel(x, y));
				}
			}
			benchmark::ClobberMemory();
		}
		
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}

	void memcpy_blit_no_alpha( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for (auto _ : aState)
		{
			for (std::uint32_t y = 0; y < source->get_height(); ++y)
			{
				std::memcpy(
					surface.get_surface_ptr() + y * surface.get_stride(),
					source->get_image_ptr() + y * source->get_stride(),
					source->get_width() * 4
				);
			}
			benchmark::ClobberMemory();
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}
}

BENCHMARK( default_blit_earth_ )
	->Args( { 320, 240 } )
	->Args( { 1280, 720 } )
	->Args( { 1920, 1080 } )
	->Args( { 7680, 4320 } );

BENCHMARK( simple_blit_no_alpha )
	->Args( { 320, 240 } )
	->Args( { 1280, 720 } )
	->Args( { 1920, 1080 } )
	->Args( { 7680, 4320 } );

BENCHMARK( memcpy_blit_no_alpha )
	->Args( { 320, 240 } )
	->Args( { 1280, 720 } )
	->Args( { 1920, 1080 } )
	->Args( { 7680, 4320 } );

BENCHMARK_MAIN();