#pragma once
#include <cstdint>

struct Framebuffer
{
    void set(const int x, const int y, const uint32_t color);
    void clear(const uint32_t color = 0xFF000000);

    int width;
    int height;
    std::uint32_t* pixels; // ARGB8888
};
