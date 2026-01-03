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

  SDL_SetRelativeMouseMode(SDL_TRUE);

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

bool SDLPlatform::processEvents(const double deltaTime)
{
  SDL_Event e;
  int mouseDX{ 0 };
  int mouseDY{ 0 };

  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      return false;
    }
    if (e.type == SDL_KEYUP)
    {
      if (e.key.keysym.sym == SDLK_e)
      {
        wireframeToggleCallback();
      }
      // TODO other options (backface culling?)
    }
    if (e.type == SDL_MOUSEMOTION)
    {
        mouseDX += e.motion.xrel;
        mouseDY += e.motion.yrel;
    }
  }

  const Uint8 *keys = SDL_GetKeyboardState(nullptr);
  if (keys[SDL_SCANCODE_W]) {
    forwardCallback(deltaTime);
  }
  if (keys[SDL_SCANCODE_S]) {
    backwardCallback(deltaTime);
  }
  if (keys[SDL_SCANCODE_A]) {
    leftCallback(deltaTime);
  }
  if (keys[SDL_SCANCODE_D]) {
    rightCallback(deltaTime);
  }
  if (mouseDX != 0 || mouseDY != 0)
  {
    cameraCallback(deltaTime, mouseDX, mouseDY);
  }

  mouseDX = 0;
  mouseDY = 0;
  return true;
}

void SDLPlatform::registerWireframeToggleCallback(std::function<void()> cb)
{
  wireframeToggleCallback = cb;
}

void SDLPlatform::registerForwardCallback(std::function<void(double)> cb)
{
  forwardCallback = cb;
}

void SDLPlatform::registerBackwardCallback(std::function<void(double)> cb)
{
  backwardCallback = cb;
}

void SDLPlatform::registerLeftCallback(std::function<void(double)> cb)
{
  leftCallback = cb;
}

void SDLPlatform::registerRightCallback(std::function<void(double)> cb)
{
  rightCallback = cb;
}

void SDLPlatform::registerCameraUpdateCallback(std::function<void(double,int,int)> cb) 
{
  cameraCallback = cb;
}

void SDLPlatform::shutdown()
{
  delete framebuffer_.depth;
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
