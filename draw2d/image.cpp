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
    // Get the width and height of the image
    auto img_width = aImage.get_width();
    auto img_height = aImage.get_height();

    // Loop through each pixel of the image
    for (ImageRGBA::Index y = 0; y < img_height; ++y) {
        for (ImageRGBA::Index x = 0; x < img_width; ++x) {
            // Calculate the position on the surface
            int target_x = static_cast<int>(aPosition.x) + x;
            int target_y = static_cast<int>(aPosition.y) + y;

            // Check if the target position is within the surface bounds
            if (target_x < 0 || target_x >= aSurface.get_width() ||
                target_y < 0 || target_y >= aSurface.get_height()) {
                continue; // Skip pixels outside the surface bounds
            }

            // Get the color of the current image pixel
            ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x, y);

            // Check the alpha channel (transparency); skip if below threshold
            if (pixel.a < 128) {
                continue;
            }

            // Convert the color to sRGB without alpha for the surface
            ColorU8_sRGB color_without_alpha = { pixel.r, pixel.g, pixel.b };

            // Set the pixel on the surface at the calculated position
            aSurface.set_pixel_srgb(target_x, target_y, color_without_alpha);
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
