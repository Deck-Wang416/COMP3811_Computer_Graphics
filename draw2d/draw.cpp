#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

void draw_line_solid(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor) {
    int x0 = static_cast<int>(aBegin.x);
    int y0 = static_cast<int>(aBegin.y);
    int x1 = static_cast<int>(aEnd.x);
    int y1 = static_cast<int>(aEnd.y);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Draw the current point
        if (x0 >= 0 && x0 < aSurface.get_width() && y0 >= 0 && y0 < aSurface.get_height()) {
            aSurface.set_pixel_srgb(x0, y0, aColor);
        }

        // Determine whether the destination has been reached
        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor)
{
    // Sort vertices by y-coordinate ascending
    if (aP1.y < aP0.y) std::swap(aP0, aP1);
    if (aP2.y < aP0.y) std::swap(aP0, aP2);
    if (aP2.y < aP1.y) std::swap(aP1, aP2);

    auto interpolate_x = [](Vec2f const& p1, Vec2f const& p2, float y) {
        return p1.x + (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y);
    };

    // Draw bottom-flat triangle if aP1.y == aP2.y
    if (aP1.y == aP2.y) {
        for (int y = static_cast<int>(std::ceil(aP0.y)); y <= static_cast<int>(std::floor(aP2.y)); ++y) {
            float x1 = interpolate_x(aP0, aP2, static_cast<float>(y));
            float x2 = interpolate_x(aP0, aP1, static_cast<float>(y));
            if (x1 > x2) std::swap(x1, x2);

            for (int x = static_cast<int>(std::ceil(x1)); x <= static_cast<int>(std::floor(x2)); ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(x, y, aColor);
                }
            }
        }
    }
    // Draw top-flat triangle if aP0.y == aP1.y
    else if (aP0.y == aP1.y) {
        for (int y = static_cast<int>(std::ceil(aP0.y)); y <= static_cast<int>(std::floor(aP2.y)); ++y) {
            float x1 = interpolate_x(aP0, aP2, static_cast<float>(y));
            float x2 = interpolate_x(aP1, aP2, static_cast<float>(y));
            if (x1 > x2) std::swap(x1, x2);

            for (int x = static_cast<int>(std::ceil(x1)); x <= static_cast<int>(std::floor(x2)); ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(x, y, aColor);
                }
            }
        }
    }
    // Draw general triangle
    else {
        // Bottom part
        for (int y = static_cast<int>(std::ceil(aP0.y)); y <= static_cast<int>(std::floor(aP1.y)); ++y) {
            float x1 = interpolate_x(aP0, aP1, static_cast<float>(y));
            float x2 = interpolate_x(aP0, aP2, static_cast<float>(y));
            if (x1 > x2) std::swap(x1, x2);

            for (int x = static_cast<int>(std::ceil(x1)); x <= static_cast<int>(std::floor(x2)); ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(x, y, aColor);
                }
            }
        }

        // Top part
        for (int y = static_cast<int>(std::ceil(aP1.y)); y <= static_cast<int>(std::floor(aP2.y)); ++y) {
            float x1 = interpolate_x(aP1, aP2, static_cast<float>(y));
            float x2 = interpolate_x(aP0, aP2, static_cast<float>(y));
            if (x1 > x2) std::swap(x1, x2);

            for (int x = static_cast<int>(std::ceil(x1)); x <= static_cast<int>(std::floor(x2)); ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(x, y, aColor);
                }
            }
        }
    }
}

inline
ColorU8_sRGB linear_to_srgb(ColorF const& color)
{
    auto convert = [](float c) -> std::uint8_t {
        // Clamp the value between 0.0 and 1.0
        c = std::max(0.0f, std::min(1.0f, c));
        // Apply gamma correction (assuming gamma = 2.2)
        c = std::pow(c, 1.0f / 2.2f);
        return static_cast<std::uint8_t>(c * 255.0f + 0.5f);
    };

    return ColorU8_sRGB{
        convert(color.r),
        convert(color.g),
        convert(color.b)
    };
}

void draw_triangle_interp(Surface& aSurface,
                          Vec2f aP0, Vec2f aP1, Vec2f aP2,
                          ColorF aC0, ColorF aC1, ColorF aC2)
{
    // Calculate the bounding box of the triangle
    float minX = std::min({ aP0.x, aP1.x, aP2.x });
    float maxX = std::max({ aP0.x, aP1.x, aP2.x });
    float minY = std::min({ aP0.y, aP1.y, aP2.y });
    float maxY = std::max({ aP0.y, aP1.y, aP2.y });

    // Limit the boundaries to the screen
    int startX = std::max(0, static_cast<int>(std::floor(minX)));
    int endX = std::min(static_cast<int>(aSurface.get_width() - 1), static_cast<int>(std::ceil(maxX)));
    int startY = std::max(0, static_cast<int>(std::floor(minY)));
    int endY = std::min(static_cast<int>(aSurface.get_height() - 1), static_cast<int>(std::ceil(maxY)));

    // Precalculate the area of ​​the triangle
    float area = (aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (aP1.y - aP0.y);
    if (area == 0.0f) return;

    // Traverse each pixel within the bounding box
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            // Current pixel center position
            float px = x + 0.5f;
            float py = y + 0.5f;

            // Calculate the center of gravity coordinates
            float w0 = ((aP1.x - aP2.x) * (py - aP2.y) + (aP2.y - aP1.y) * (px - aP2.x)) / area;
            float w1 = ((aP2.x - aP0.x) * (py - aP2.y) + (aP0.y - aP2.y) * (px - aP2.x)) / area;
            float w2 = 1.0f - w0 - w1;

            // Determine whether the pixel is inside the triangle
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                // Color interpolation
                ColorF color = {
                    w0 * aC0.r + w1 * aC1.r + w2 * aC2.r,
                    w0 * aC0.g + w1 * aC1.g + w2 * aC2.g,
                    w0 * aC0.b + w1 * aC1.b + w2 * aC2.b
                };

                // Convert linear RGB to sRGB
                ColorU8_sRGB srgb_color = linear_to_srgb(color);

                aSurface.set_pixel_srgb(x, y, srgb_color);
            }
        }
    }
}

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
