#pragma once
#include "iplatform.hpp"
#include "object.hpp"
#include "camera.hpp"

class Renderer
{
public:
  void render(Framebuffer& fb, const Object& object);
  void toggleWireframe();
  void setBackfaceCulling(const bool enable);
  Camera& getCamera();
private:
  bool showWireframe{false};
  bool doBackfaceCulling{true};
  Camera cam;
};