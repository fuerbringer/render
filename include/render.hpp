#pragma once
#include "iplatform.hpp"
#include "object.hpp"
#include "camera.hpp"

class Renderer
{
public:
  void render(Framebuffer& fb, Camera& cam, const Object& object);
  void toggleWireframe();
  void setBackfaceCulling(const bool enable);
private:
  bool showWireframe{false};
  bool doBackfaceCulling{true};
};