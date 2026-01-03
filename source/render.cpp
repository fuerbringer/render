#include "render.hpp"
#include "vector.hpp"
#include <vector>
#include <cmath>

namespace {

inline Vec2f transformViewportToScreen(const Vec2f p, const int width, const int height)
{
    return {
      (p.x + 1) / 2 * width,
      (1 - (p.y + 1) / 2) * height
    };
}

inline Vec2f projectToScreen(const Vec3f p)
{
  return { p.x / p.z, p.y / p.z };
}

void drawLine(
    Framebuffer& fb,
    int x0, int y0,
    int x1, int y1,
    uint32_t color
)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        fb.set(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

inline Vec2f project(Framebuffer& fb, const Vec3f p)
{
  auto projectedPixel = projectToScreen(p);
  auto screenPixel = transformViewportToScreen(projectedPixel, fb.width, fb.height);
  return screenPixel;
}

}

void Renderer::render(Framebuffer& fb, const Object& object)
{
  auto vertices = object.getTransformedVertices();
  auto& faces = object.faces;
  for(size_t f = 0; f < faces.size(); f++) {
    for(size_t i = 0; i < faces[f].size(); i++) {
      const auto a = project(fb, vertices[faces[f][i]]);
      const auto b = project(fb, vertices[faces[f][(i + 1) % faces[f].size()]]);

      drawLine(fb, a.x, a.y, b.x, b.y, 0xFFFFFFFF);
    }
  }
}
