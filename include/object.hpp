#pragma once
#include <array>
#include <vector>
#include "vector.hpp"

using Vertices = std::vector<Vec3f>;
using Face = Vec3i; // triangle
using Faces = std::vector<Face>;

struct Transform
{
    void translate(const Vec3f& delta);
    void rotate(const Vec3f& radians);
    void scale(const Vec3f& factor);

    Vec3f position{0, 0, 0};
    Vec3f rotation{0, 0, 0};
    Vec3f scaling{1, 1, 1};
};

struct Object 
{
    virtual void update(const double deltaTime);
    Vertices getTransformedVertices() const;

    Vertices vertices;
    Faces faces;
    Transform transform;
};