#include "framebuffer.hpp"
#include <algorithm>
#include <limits>

void Framebuffer::set(
    const int x,
    const int y,
    const double z,
    const uint32_t color)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height)
    {
        return; // clip
    }
    const int index = y * width + x;
    if (z < depth[index])
    {
        depth[index] = z;
        pixels[index] = color;
    }
}

void Framebuffer::clear(const uint32_t color)
{
    std::fill(pixels, pixels + width * height, color);
    std::fill(depth, depth + width * height, std::numeric_limits<double>::infinity());
}