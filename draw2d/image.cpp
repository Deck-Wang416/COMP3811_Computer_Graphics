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

void blit_masked(Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition)
{
    // Calculate the starting position on the surface
    int startX = static_cast<int>(std::round(aPosition.x));
    int startY = static_cast<int>(std::round(aPosition.y));

    // Loop through each pixel in the image
    for (ImageRGBA::Index y = 0; y < aImage.get_height(); ++y)
    {
        for (ImageRGBA::Index x = 0; x < aImage.get_width(); ++x)
        {
            // Get the pixel color from the image
            ColorU8_sRGB_Alpha pixelColor = aImage.get_pixel(x, y);

            // Skip the pixel if the alpha value is less than 128 (considered transparent)
            if (pixelColor.a < 128)
            {
                continue;
            }

            // Calculate the position on the surface
            int surfaceX = startX + static_cast<int>(x);
            int surfaceY = startY + static_cast<int>(y);

            // Ensure the position is within the bounds of the surface
            if (surfaceX >= 0 && surfaceX < static_cast<int>(aSurface.get_width()) &&
                surfaceY >= 0 && surfaceY < static_cast<int>(aSurface.get_height()))
            {
                // Set the pixel color on the surface
                aSurface.set_pixel_srgb(
                    static_cast<Surface::Index>(surfaceX),
                    static_cast<Surface::Index>(surfaceY),
                    { pixelColor.r, pixelColor.g, pixelColor.b }
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
