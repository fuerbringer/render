#include "framebuffer.hpp"

void Framebuffer::set(
    const int x,
    const int y,
    const uint32_t color)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height)
    {
        return; // clip
    }
    pixels[y * width + x] = color;
}