#pragma once
#include "iplatformgraphics.hpp"
#include <SDL2/SDL.h>

class SDLGraphics final : public IPlatformGraphics
{
public:
    SDLGraphics();
    ~SDLGraphics();

    bool initialize(int width, int height, std::string_view title) override;

    Framebuffer& framebuffer() override;

    void present();

    bool processEvents() override;

    void shutdown() override;

private:
    SDL_Window* window_ = nullptr;
    SDL_Surface* surface_ = nullptr;
    Framebuffer framebuffer_{};
};

