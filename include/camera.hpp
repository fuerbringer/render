#pragma once
#include "vector.hpp"

struct Camera
{
    Vec3f forward();

    Vec3f right();

    Vec3f up();

    void updateCamera(double dt, double dx, double dy);

    Vec3f position { 0, 0, 0 };

    double yaw   { 0.0 }; // left/right (around Y axis)
    double pitch { 0.0 }; // up/down   (around X axis)

    double fov { 90.0 };  // degrees
    double near { 0.1 };
    double far  { 100.0 };
};