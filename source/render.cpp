#include "render.hpp"
#include "vector.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

namespace {

inline Vec3f transformViewportToScreen(const Vec3f p, const int width, const int height)
{
    return {
      (p.x + 1) / 2 * width,
      (1 - (p.y + 1) / 2) * height,
      p.z
    };
}

inline Vec3f projectToScreen(const Vec3f p)
{
  return { p.x / p.z, p.y / p.z, p.z };
}

inline void drawLine(
    Framebuffer& fb,
    const Vec3f a,
    const Vec3f b,
    const uint32_t color
)
{
    const int dx = abs(b.x - a.x);
    const int dy = abs(b.y - a.y);

    const int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i)
    {
        const auto t = (steps == 0) ? 0 : static_cast<double>(i) / steps;
        const auto x = int(a.x + t * (b.x - a.x));
        const auto y = int(a.y + t * (b.y - a.y));
        const auto z = a.z + t * (b.z - a.z);
        fb.set(x, y, z, color);
    }
}


inline Vec3f project(Framebuffer& fb, const Vec3f p)
{
  auto projectedPixel = projectToScreen(p);
  auto screenPixel = transformViewportToScreen(projectedPixel, fb.width, fb.height);
  return screenPixel;
}

inline bool insideTriangle(const Vec3f& p, const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
    const auto w0 = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    const auto w1 = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
    const auto w2 = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);

    return (w0 >= 0 && w1 >= 0 && w2 >= 0) ||
           (w0 <= 0 && w1 <= 0 && w2 <= 0);
}

inline Vec3f barycentric(const Vec3f& p, const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
    const auto denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    const auto alpha = ((b.y - c.y) *(p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
    const auto beta  = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
    const auto gamma = 1.0 - alpha - beta;
    return Vec3f(alpha, beta, gamma); // z here is unused
}

inline void drawFilledTriangle(
    Framebuffer& fb,
    const Vec3f p0,
    const Vec3f p1,
    const Vec3f p2,
    const uint32_t color)
{
    const int minX = std::min({int(p0.x), int(p1.x), int(p2.x)});
    const int maxX = std::max({int(p0.x), int(p1.x), int(p2.x)});
    const int minY = std::min({int(p0.y), int(p1.y), int(p2.y)});
    const int maxY = std::max({int(p0.y), int(p1.y), int(p2.y)});

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            Vec3f p{x + 0.5f, y + 0.5f, 0}; // z will be interpolated

            if (insideTriangle(p, p0, p1, p2))
            {
                // compute barycentric coords
                const auto bc = barycentric(p, p0, p1, p2);

                // interpolate z
                const auto z = bc.x * p0.z + bc.y * p1.z + bc.z * p2.z;

                fb.set(x, y, z, color);
            }
        }
    }
}

inline uint32_t computeFlatLambertShading(const Vec3f &normal)
{
  constexpr auto ambient { 0.2 };
  // hardcoded light
  constexpr auto lightDir { normalize(Vec3f{0.5f, 1.0f, -0.3f}) };
  auto intensity = dot(normal, lightDir);
  intensity = std::fmax(0, intensity);
  intensity = ambient + intensity * (1.0 - ambient);
  const uint8_t c = static_cast<uint8_t>(intensity * 255);
  const uint32_t color =
      (255 << 24) | // alpha
      (c << 16) |   // red
      (c << 8) |    // green
      c;            // blue
  return color;
}
}

void Renderer::render(Framebuffer& fb, const Object& object)
{
    auto vertices { object.getTransformedVertices() };
    const auto& faces { object.faces };

    for (auto face : faces)
    {
        // vertices in world space
        const auto v0 { vertices[face[0]] };
        const auto v1 { vertices[face[1]] };
        const auto v2 { vertices[face[2]] };

        const auto e1 { v1 - v0 };
        const auto e2 { v2 - v0 };
        const auto normal { normalize(cross(e1, e2)) };

        // TODO fix magic number (why does 0.3 work better than 0)
        if (doBackfaceCulling && normal.z > 0.3) { 
            continue;
        }

        const auto color { computeFlatLambertShading(normal) };

        // project after culling
        const auto p0 { project(fb, v0) };
        const auto p1 { project(fb, v1) };
        const auto p2 { project(fb, v2) };

        if(showWireframe) {
            drawLine(fb, p0, p1, WHITE);
            drawLine(fb, p1, p2, WHITE);
            drawLine(fb, p2, p0, WHITE);
        } else {
            drawFilledTriangle(fb, p0, p1, p2, color);
        }
    }
}

void Renderer::toggleWireframe()
{
    showWireframe = !showWireframe;
}

void Renderer::setBackfaceCulling(const bool enable)
{
    doBackfaceCulling = enable;
}