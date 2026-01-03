#pragma once
#include <cassert>
#include <cstddef>

template <typename T>
struct Vec3
{

  Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

  Vec3<T> cross(const Vec3<T> &b)
  {
    return {
        y * b.z - z * b.y,
        z * b.x - x * b.z,
        x * b.y - y * b.x
    };
  }

  T& operator[](const size_t i)
  {
    switch (i)
    {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      assert(0);
      return z;
    }
    return z;
  }

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
Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

template <typename T>
struct Vec2
{
  Vec2(T x_, T y_) : x(x_), y(y_) {}

  T x{};
  T y{};
};

template <typename T>
T cross(const Vec2<T> &a, const Vec2<T> &b)
{
  return a.x * b.y - a.y * b.x;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b)
{
    return { a.x - b.x, a.y - b.y };
}

using Vec3f = Vec3<double>;
using Vec3i = Vec3<int>;
using Vec2f = Vec2<double>;
using Vec2i = Vec2<int>;