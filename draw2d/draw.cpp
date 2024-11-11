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

void draw_triangle_solid(Surface& surface, Vec2f p0, Vec2f p1, Vec2f p2, ColorU8_sRGB color) {
    // Sort vertices by y-coordinate
    sort_vertices_by_y(p0, p1, p2);

    // Calculate slopes for the left and right edges of the triangle
    float slope1 = (p1.y == p0.y) ? 0 : (p1.x - p0.x) / (p1.y - p0.y);
    float slope2 = (p2.y == p0.y) ? 0 : (p2.x - p0.x) / (p2.y - p0.y);
    float slope3 = (p2.y == p1.y) ? 0 : (p2.x - p1.x) / (p2.y - p1.y);

    // Draw the top part of the triangle
    for (int y = static_cast<int>(p0.y); y < static_cast<int>(p1.y); ++y) {
        int x_start = static_cast<int>(p0.x + slope1 * (y - p0.y));
        int x_end = static_cast<int>(p0.x + slope2 * (y - p0.y));
        draw_horizontal_line(surface, y, x_start, x_end, color);
    }

    // Draw the bottom part of the triangle
    for (int y = static_cast<int>(p1.y); y <= static_cast<int>(p2.y); ++y) {
        int x_start = static_cast<int>(p1.x + slope3 * (y - p1.y));
        int x_end = static_cast<int>(p0.x + slope2 * (y - p0.y));
        draw_horizontal_line(surface, y, x_start, x_end, color);
    }
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;      // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aC0;
	(void)aC1;
	(void)aC2;
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
