inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel(Index aX, Index aY) const
{
    assert(aX < mWidth && aY < mHeight);

    // Calculate the linear index in the mData array
    Index linearIndex = (aY * mWidth + aX) * 4;

    // Retrieve color components from mData
    return ColorU8_sRGB_Alpha{
        mData[linearIndex],       // Red
        mData[linearIndex + 1],   // Green
        mData[linearIndex + 2],   // Blue
        mData[linearIndex + 3]    // Alpha
    };
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
    // Assert boundary conditions
    assert(aX < mWidth && aY < mHeight);
    
    // Calculate linear index assuming row-major order
    return aY * mWidth + aX;
}
