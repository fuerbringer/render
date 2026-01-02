#include <chrono>
#include <thread>
#include <memory>
#include <algorithm>

#include "sdlgraphics.hpp"
#include "render.hpp"

constexpr double TARGET_FPS = 30.0;
constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

int main()
{
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

        // ---- Render ----
        Framebuffer& fb = gfx->framebuffer();

        std::fill(
            fb.pixels,
            fb.pixels + fb.width * fb.height,
            0xFF000000
        );

        render(fb, deltaTime);

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

