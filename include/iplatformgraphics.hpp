#pragma once
#include <string_view>
#include "framebuffer.hpp"

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

