#pragma once

template <typename T>
struct Vec3
{
  T x{};
  T y{};
  T z{};
};

template <typename T>
Vec3<T>& operator+=(Vec3<T>& a, const Vec3<T>& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template <typename T>
Vec3<T>& operator*=(Vec3<T>& a, const Vec3<T>& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

template <typename T>
struct Vec2
{
  T x{};
  T y{};
};

using Vec3f = Vec3<double>;
using Vec3i = Vec3<int>;
using Vec2f = Vec2<double>;
using Vec2i = Vec2<int>;