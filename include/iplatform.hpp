#pragma once
#include <string_view>
#include <functional>
#include "framebuffer.hpp"

class IPlatform
{
public:
    virtual ~IPlatform() = default;

    virtual bool initialize(
        int width,
        int height,
        std::string_view title
    ) = 0;

    virtual Framebuffer& framebuffer() = 0;

    virtual void present() = 0;

    // returns false when the user wants to quit
    virtual bool processEvents() = 0;

    virtual void registerWireframeToggleCallback(std::function<void()> cb) = 0;

    virtual void shutdown() = 0;
};

