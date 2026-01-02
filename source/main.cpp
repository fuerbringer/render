#include <chrono>
#include <thread>
#include <memory>
#include <algorithm>

#include "sdlgraphics.hpp"
#include "render.hpp"

constexpr double TARGET_FPS = 30.0;
constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

Object getCube()
{
  Object object;
  object.vertices.push_back({ 0.25,  0.25, 0.25});
  object.vertices.push_back({-0.25,  0.25, 0.25});
  object.vertices.push_back({-0.25, -0.25, 0.25});
  object.vertices.push_back({ 0.25, -0.25, 0.25});
  object.vertices.push_back({ 0.25,  0.25, -0.25});
  object.vertices.push_back({-0.25,  0.25, -0.25});
  object.vertices.push_back({-0.25, -0.25, -0.25});
  object.vertices.push_back({ 0.25, -0.25, -0.25});
  object.faces.push_back({0, 1, 2, 3});
  object.faces.push_back({4, 5, 6, 7});
  object.faces.push_back({0, 4});
  object.faces.push_back({1, 5});
  object.faces.push_back({2, 6});
  object.faces.push_back({3, 7});
  object.transform.translate({0, 0, 1});
  return object;
}

void rotateCube(Object& cube, const double delta)
{
    cube.transform.rotate({ delta, delta, delta});
}

int main()
{
    auto cube = getCube();
    Renderer renderer;

    std::unique_ptr<IPlatformGraphics> gfx =
        std::make_unique<SDLGraphics>();

    if (!gfx->initialize(800, 600, "Software Renderer"))
        return -1;

    using clock = std::chrono::steady_clock;

    auto lastTime = clock::now();

    bool running = true;
    while (running)
    {
        auto frameStart = clock::now();

        // ---- Time step ----
        std::chrono::duration<double> delta =
            frameStart - lastTime;
        lastTime = frameStart;

        double deltaTime = delta.count();

        // Clamp deltaTime (prevents huge jumps if debugger pauses)
        if (deltaTime > 0.25)
            deltaTime = 0.25;

        // ---- Events ----
        running = gfx->processEvents();

        // ---- Update ----
        // TODO
        //updateScene(deltaTime);
        rotateCube(cube, M_PI * deltaTime);

        // ---- Render ----
        Framebuffer& fb = gfx->framebuffer();

        std::fill(
            fb.pixels,
            fb.pixels + fb.width * fb.height,
            0xFF000000
        );

        renderer.render(fb, cube);

        gfx->present();

        // ---- Frame pacing ----
        auto frameEnd = clock::now();
        std::chrono::duration<double> frameDuration =
            frameEnd - frameStart;

        if (frameDuration.count() < TARGET_FRAME_TIME)
        {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(
                    TARGET_FRAME_TIME - frameDuration.count()
                )
            );
        }
    }

    gfx->shutdown();
    return 0;
}

