#include "camera.hpp"
#include <algorithm>
#include <cmath>

Vec3f Camera::forward()
{
    return normalize(Vec3f{
        std::cos(pitch) * sinf(yaw),
        std::sin(pitch),
        std::cos(pitch) * cosf(yaw)
    });
}

Vec3f Camera::right()
{
    return normalize(cross(forward(), {0, 1, 0}));
}

Vec3f Camera::up()
{
    return cross(right(), forward());
}

void Camera::updateCamera(double dt, double dx, double dy)
{
    constexpr auto sensitivity { 0.01 };

    yaw   += dt * dx * sensitivity;
    pitch -= dt * dy * sensitivity;

    pitch = std::clamp(pitch, -1.5, 1.5);
}
