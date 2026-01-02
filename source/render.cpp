#include "render.hpp"
#include <vector>
#include <cmath>

static Pixel2 transformViewportToScreen(const Pixel2 p, const int width, const int height)
{
    return {
      (p.x + 1) / 2 * width,
      (1 - (p.y + 1) / 2) * height
    };
}

static Pixel2 projectToScreen(const Point3 p)
{
  return { p.x / p.z, p.y / p.z };
}

static void drawLine(
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

static Pixel2 project(Framebuffer& fb, const Point3 p)
{

  auto projectedPixel = projectToScreen(p);
  return transformViewportToScreen(projectedPixel, fb.width, fb.height);
}

/*
static void renderPoint(Framebuffer& fb, const Point3 p)
{
  auto screenPixel = project(fb, p);
  fb.set(screenPixel.x, screenPixel.y, 0xFFFFFFFF);
}
*/

static void renderObject(Framebuffer& fb, const std::vector<Point3>& vertices, const std::vector<std::vector<int>>& faces)
{
  for(size_t f = 0; f < faces.size(); f++) {
    for(size_t i = 0; i < faces[f].size(); i++) {
      auto a = project(fb, vertices[faces[f][i]]);
      auto b = project(fb, vertices[faces[f][(i + 1) % faces[f].size()]]);

      drawLine(fb, a.x, a.y, b.x, b.y, 0xFFFFFFFF);
    }
  }
}

static Point3 translate_z(Point3 p, double dz)
{
  p.z += dz;
  return p;
}

static Point3 rotate_xz(const Point3 p, double angle)
{
    const auto c = std::cos(angle);
    const auto s = std::sin(angle);
    return {
        p.x * c - p.z * s,
        p.y,
        p.x * s + p.z * c
    };
}

void render(Framebuffer& fb, double deltaTime)
{
  static double angle = 0;
  angle += M_PI * deltaTime / 2;

  std::vector<Point3> cube;
  cube.push_back({ 0.25,  0.25, 0.25});
  cube.push_back({-0.25,  0.25, 0.25});
  cube.push_back({-0.25, -0.25, 0.25});
  cube.push_back({ 0.25, -0.25, 0.25});
  cube.push_back({ 0.25,  0.25, -0.25});
  cube.push_back({-0.25,  0.25, -0.25});
  cube.push_back({-0.25, -0.25, -0.25});
  cube.push_back({ 0.25, -0.25, -0.25});
  std::vector<std::vector<int>> faces;
  faces.push_back({0, 1, 2, 3});
  faces.push_back({4, 5, 6, 7});
  faces.push_back({0, 4});
  faces.push_back({1, 5});
  faces.push_back({2, 6});
  faces.push_back({3, 7});

  for(auto& p : cube) {
    p = rotate_xz(p, angle);
    p = translate_z(p, 1);
  }

  renderObject(fb, cube, faces);

  /*
  std::vector<Point3> obj;
  obj.push_back({ 0.25,  0.25, 1});
  obj.push_back({ -0.25,  0.25, 1});
  obj.push_back({ 0.25,  -0.25, 1});
  obj.push_back({ -0.25, -0.25, 1});
  std::vector<std::vector<int>> faces;
  faces.push_back({0, 1, 2}),
  renderObject(fb, obj, faces);
  */
}
