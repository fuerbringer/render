#include "sdlplatform.hpp"
#include <iostream>

SDLPlatform::SDLPlatform() {}
SDLPlatform::~SDLPlatform() {}

bool SDLPlatform::initialize(const int width, const int height, const std::string_view title) 
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return false;

  window_ = SDL_CreateWindow(
      title.data(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width, height,
      0
      );

  surface_ = SDL_GetWindowSurface(window_);

  framebuffer_.width  = width;
  framebuffer_.height = height;
  framebuffer_.pixels =
    static_cast<std::uint32_t*>(surface_->pixels);

  framebuffer_.depth = new double[width * height];

  if(framebuffer_.pixels == nullptr) {
    return false;
  }
  if(framebuffer_.depth == nullptr) {
    return false;
  }

  return true;
}

Framebuffer& SDLPlatform::framebuffer()
{
  return framebuffer_;
}

void SDLPlatform::present()
{
  SDL_UpdateWindowSurface(window_);
}

bool SDLPlatform::processEvents()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT) {
      return false;
    } else if (e.type == SDL_KEYUP) {
      if (e.key.keysym.sym == SDLK_e) {
        wireframeToggleCallback();
      }
      // TODO other options (backface culling?)
    }
  }
  return true;
}

void SDLPlatform::registerWireframeToggleCallback(std::function<void()> cb)
{
  wireframeToggleCallback = cb;
}

void SDLPlatform::shutdown()
{
  delete framebuffer_.depth;
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
