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

    void present() override;

    bool processEvents(const double deltaTime) override;

    void registerWireframeToggleCallback(std::function<void()> cb) override;
    void registerForwardCallback(std::function<void(double)> cb) override;
    void registerBackwardCallback(std::function<void(double)> cb) override;
    void registerLeftCallback(std::function<void(double)> cb) override;
    void registerRightCallback(std::function<void(double)> cb) override;
    void registerCameraUpdateCallback(std::function<void(double,int,int)> cb) override;

    void shutdown() override;

private:
    SDL_Window* window_{nullptr};
    SDL_Surface* surface_{nullptr};
    Framebuffer framebuffer_{};
    std::function<void()> wireframeToggleCallback{};
    std::function<void(double)> forwardCallback{};
    std::function<void(double)> backwardCallback{};
    std::function<void(double)> leftCallback{};
    std::function<void(double)> rightCallback{};
    std::function<void(double,int,int)> cameraCallback{};
};

