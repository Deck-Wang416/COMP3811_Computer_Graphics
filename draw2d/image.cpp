#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked(Surface& targetSurface, ImageRGBA const& sourceImage, Vec2f position)
{
    // Determine the starting position on the target surface
    int destX = static_cast<int>(std::round(position.x));
    int destY = static_cast<int>(std::round(position.y));

    // Traverse through each pixel in the source image
    for (ImageRGBA::Index row = 0; row < sourceImage.get_height(); ++row)
    {
        for (ImageRGBA::Index col = 0; col < sourceImage.get_width(); ++col)
        {
            // Get color and alpha, skip if transparent
            ColorU8_sRGB_Alpha srcPixel = sourceImage.get_pixel(col, row);

            if (srcPixel.a < 128) continue;

            // Calculate the target position and check bounds
            int targetX = destX + static_cast<int>(col);
            int targetY = destY + static_cast<int>(row);

            if (targetX >= 0 && targetX < static_cast<int>(targetSurface.get_width()) &&
                targetY >= 0 && targetY < static_cast<int>(targetSurface.get_height()))
            {
                // Set the pixel on the surface with RGB values
                targetSurface.set_pixel_srgb(
                    static_cast<Surface::Index>(targetX),
                    static_cast<Surface::Index>(targetY),
                    { srcPixel.r, srcPixel.g, srcPixel.b }
                );
            }
        }
    }
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
