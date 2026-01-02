#pragma once
#include "i_platformgraphics.hpp"

// generic 3d vector
struct Vec3
{
  double x{0};
  double y{0};
  double z{0};
};

// generic 2d vector
struct Vec2
{
  double x{0};
  double y{0};
};

// object space point (3d)
struct Point3 : public Vec3
{ };

// pixel screen space (2d)
struct Pixel2 : public Vec2
{ };

void render(Framebuffer& fb, double deltaTime);
