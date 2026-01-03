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

inline void drawLineDepth(
    Framebuffer& fb,
    const Vec3f a,
    const Vec3f b,
    const uint32_t color
)
{
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i)
    {
        double t = (steps == 0) ? 0 : (double)i / steps;
        int x = int(a.x + t * (b.x - a.x));
        int y = int(a.y + t * (b.y - a.y));
        double z = a.z + t * (b.z - a.z);
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
    double w0 = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    double w1 = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
    double w2 = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);

    return (w0 >= 0 && w1 >= 0 && w2 >= 0) ||
           (w0 <= 0 && w1 <= 0 && w2 <= 0);
}

inline Vec3f barycentric(const Vec3f& p, const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
    double denom = (b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y);
    double alpha = ((b.y - c.y)*(p.x - c.x) + (c.x - b.x)*(p.y - c.y)) / denom;
    double beta  = ((c.y - a.y)*(p.x - c.x) + (a.x - c.x)*(p.y - c.y)) / denom;
    double gamma = 1.0f - alpha - beta;
    return Vec3f(alpha, beta, gamma); // z here is unused
}

inline void drawFilledTriangle(
    Framebuffer& fb,
    const Vec3f p0,
    const Vec3f p1,
    const Vec3f p2,
    uint32_t color)
{
    int minX = std::min({int(p0.x), int(p1.x), int(p2.x)});
    int maxX = std::max({int(p0.x), int(p1.x), int(p2.x)});
    int minY = std::min({int(p0.y), int(p1.y), int(p2.y)});
    int maxY = std::max({int(p0.y), int(p1.y), int(p2.y)});

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            Vec3f p(x + 0.5f, y + 0.5f, 0); // z will be interpolated

            if (insideTriangle(p, p0, p1, p2))
            {
                // compute barycentric coords
                const Vec3f bc = barycentric(p, p0, p1, p2);

                // interpolate z
                const double z = bc.x * p0.z + bc.y * p1.z + bc.z * p2.z;

                fb.set(x, y, z, color);
            }
        }
    }
}

static inline uint32_t computeFlatLambertShading(const Vec3f &normal)
{
  // hardcoded light
  Vec3f lightDir = normalize(Vec3f{0.5f, 1.0f, -0.3f});
  double intensity = dot(normal, lightDir);
  intensity = std::fmax(0, intensity);
  float ambient = 0.2;
  intensity = ambient + intensity * (1.0 - ambient);
  uint8_t c = static_cast<uint8_t>(intensity * 255);
  uint32_t color =
      (255 << 24) | // alpha
      (c << 16) |   // red
      (c << 8) |    // green
      c;            // blue
  return color;
}
}

void Renderer::render(Framebuffer& fb, const Object& object)
{
    auto vertices = object.getTransformedVertices();
    const auto& faces = object.faces;

    for (auto face : faces)
    {
        // indices
        int i0 = face[0];
        int i1 = face[1];
        int i2 = face[2];

        // vertices in view/world space
        Vec3f v0 = vertices[i0];
        Vec3f v1 = vertices[i1];
        Vec3f v2 = vertices[i2];

        // compute face normal
        Vec3f e1 = v1 - v0;
        Vec3f e2 = v2 - v0;
        Vec3f normal = normalize(cross(e1, e2));

        // back-face culling
        if (normal.z >= 0) {
            continue;
        }

        const auto color = computeFlatLambertShading(normal);

        // project after culling
        auto p0 = project(fb, v0);
        auto p1 = project(fb, v1);
        auto p2 = project(fb, v2);

/*
        drawLineDepth(fb, p0, p1, RED);
        drawLineDepth(fb, p1, p2, GREEN);
        drawLineDepth(fb, p2, p0, BLUE);
*/
        drawFilledTriangle(fb, p0, p1, p2, color);
    }
}

