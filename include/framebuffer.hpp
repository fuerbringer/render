#pragma once
#include <cstdint>

constexpr uint32_t BLACK = 0xFF000000;
constexpr uint32_t WHITE = 0xFFFFFFFF;
constexpr uint32_t RED   = 0xFFFF0000;
constexpr uint32_t GREEN = 0xFF00FF00;
constexpr uint32_t BLUE  = 0xFF0000FF;

struct Framebuffer
{
    void set(const int x, const int y, const double z, const uint32_t color);
    void clear(const uint32_t color = BLACK);

    int width;
    int height;
    std::uint32_t* pixels{nullptr}; // ARGB8888
    double* depth{nullptr}; // z-buffer
};
