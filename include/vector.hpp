#pragma once
#include <cassert>
#include <cstddef>
#include <cmath>

template <typename T>
struct Vec3
{
  constexpr Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

  constexpr Vec3<T> cross(const Vec3<T> &b)
  {
    return {
        y * b.z - z * b.y,
        z * b.x - x * b.z,
        x * b.y - y * b.x
    };
  }

  constexpr T& operator[](const size_t i)
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
constexpr Vec3<T>& operator+=(Vec3<T>& a, const Vec3<T>& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template <typename T>
constexpr Vec3<T>& operator*=(Vec3<T>& a, const Vec3<T>& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

template <typename T>
constexpr Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

template <typename T>
constexpr Vec3<T> operator/(const Vec3<T>& a, const double d)
{
    return { a.x / d, a.y / d, a.z / d };
}

template <typename T>
constexpr Vec3<T> operator3(Vec3<T> a, double d)
{
    return { a.x * d, a.y * d, a.z * d };
}

template <typename T>
constexpr T length(const Vec3<T>& v)
{
    return std::sqrt(dot(v, v));
}


template <typename T>
Vec3<T> normalize( Vec3<T>&& v)
{
    T len { length(v) };
    if (len == T(0)) {
      return v;
    }
    return v / len;
}

template <typename T>
constexpr Vec3<T> normalize(const Vec3<T>& v)
{
    T len { length(v) };
    if (len == T(0)) {
      return v;
    }
    return v / len;
}

template <typename T>
constexpr T dot(const Vec3<T>& a, const Vec3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
constexpr Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b)
{
  return {
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x
  };
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