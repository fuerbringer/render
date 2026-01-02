#pragma once
#include <cstdint>
#include <string_view>

struct Framebuffer
{
    int width;
    int height;
    std::uint32_t* pixels; // ARGB8888

    inline void set(
        const int x,
        const int y,
        const uint32_t color
        )
    {
      if (x < 0 || x >= width ||
          y < 0 || y >= height) {
        return; // clip
      }
      pixels[y * width + x] = color;
    }
};

class IPlatformGraphics
{
public:
    virtual ~IPlatformGraphics() = default;

    virtual bool initialize(
        int width,
        int height,
        std::string_view title
    ) = 0;

    virtual Framebuffer& framebuffer() = 0;

    virtual void present() = 0;

    // returns false when the user wants to quit
    virtual bool processEvents() = 0;

    virtual void shutdown() = 0;
};

