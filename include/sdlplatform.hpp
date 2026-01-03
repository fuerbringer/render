#pragma once
#include "iplatform.hpp"
#include <SDL2/SDL.h>

class SDLPlatform final : public IPlatform
{
public:
    SDLPlatform();
    ~SDLPlatform();

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

