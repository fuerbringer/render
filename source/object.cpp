#include "object.hpp"
#include <cmath>

namespace
{
    inline Vec3f rotateX(const Vec3f& v, const float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            v.x,
            v.y * c - v.z * s,
            v.y * s + v.z * c
        };
    };
    inline Vec3f rotateY(const Vec3f& v, const float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            v.x * c + v.z * s,
            v.y,
            -v.x * s + v.z * c
        };
    }
    inline Vec3f rotateZ(const Vec3f &v, const float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            v.x * c - v.y * s,
            v.x * s + v.y * c,
            v.z
        };
    }
}

void Transform::translate(const Vec3f &delta)
{
    position += delta;
}

void Transform::rotate(const Vec3f &radians)
{
    rotation += radians;
}

void Transform::scale(const Vec3f &factor)
{
    scaling *= factor;
}

void Object::update(const double deltaTime)
{
    (void)deltaTime;
}

Vertices Object::getTransformedVertices() const
{
    auto vtrans = vertices;
    for(Vec3f& v : vtrans) {
        // scale
        v *= transform.scaling;

        // rotate
        v = rotateX(v, transform.rotation.x);
        v = rotateY(v, transform.rotation.y);
        v = rotateZ(v, transform.rotation.z);

        // translate
        v += transform.position;
    }
    return vtrans;
}