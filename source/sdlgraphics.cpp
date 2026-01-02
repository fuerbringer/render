#include "sdlgraphics.hpp"

SDLGraphics::SDLGraphics() {}
SDLGraphics::~SDLGraphics() {}

bool SDLGraphics::initialize(const int width, const int height, const std::string_view title) 
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

  return true;
}

Framebuffer& SDLGraphics::framebuffer()
{
  return framebuffer_;
}

void SDLGraphics::present()
{
  SDL_UpdateWindowSurface(window_);
}

bool SDLGraphics::processEvents()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      return false;
  }
  return true;
}

void SDLGraphics::shutdown()
{
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
