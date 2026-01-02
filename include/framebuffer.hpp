#pragma once
#include <cstdint>

struct Framebuffer
{
    void set(const int x, const int y, const uint32_t color);

    int width;
    int height;
    std::uint32_t* pixels; // ARGB8888
};
