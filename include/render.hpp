#pragma once
#include "iplatformgraphics.hpp"
#include "object.hpp"

class Renderer
{
public:
  void render(Framebuffer& fb, const Object& object);
};