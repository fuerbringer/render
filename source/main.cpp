#include <chrono>
#include <thread>
#include <memory>

#include "sdlplatform.hpp"
#include "render.hpp"
#include "demoobjects.hpp"
#include "camera.hpp"

constexpr double TARGET_FPS = 24.0;
constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

// temp. helper
void rotateObj(Object& obj, const double delta)
{
    obj.transform.rotate({ delta, delta, delta});
}

int main()
{
    std::array<Object, 2> objs { getCube(), getPenger() };
    Renderer renderer;
    Camera cam;

    std::unique_ptr<IPlatform> platform { std::make_unique<SDLPlatform>() }; 

    if (!platform->initialize(800, 600, "Software Renderer")) {
        return -1;
    }
    platform->registerWireframeToggleCallback([&](){
        renderer.toggleWireframe();
    });
    platform->registerForwardCallback([&](double dt){
        auto fwd = cam.forward();
        fwd *= {dt,dt,dt};
        cam.position += fwd;
    });
    platform->registerBackwardCallback([&](double dt){
        auto bwd = cam.forward();
        bwd *= {-dt,-dt,-dt};
        cam.position += bwd;
    });
    platform->registerRightCallback([&](double dt){
        auto r = cam.right();
        r *= {-dt,-dt,-dt};
        cam.position += r;
    });
    platform->registerLeftCallback([&](double dt){
        auto l = cam.right();
        l *= {dt,dt,dt};
        cam.position += l;
    });
    platform->registerCameraUpdateCallback([&](double dt, int dx, int dy){
        cam.updateCamera(dt, dx, dy);
    });

    using clock = std::chrono::steady_clock;

    auto lastTime{ clock::now() };
    auto running { true };

        renderer.toggleWireframe();
    while (running)
    {
        auto frameStart = clock::now();

        // ---- Time step ----
        std::chrono::duration<double> delta { frameStart - lastTime };
        lastTime = frameStart;

        auto deltaTime { delta.count() };

        // Clamp deltaTime (prevents huge jumps if debugger pauses)
        if (deltaTime > 0.25)
            deltaTime = 0.25;

        // ---- Events ----
        running = platform->processEvents(deltaTime);

        // ---- Update ----
        for(auto& obj : objs) {
            rotateObj(obj, M_PI * deltaTime / 10);
        }

        // ---- Render ----
        auto& fb { platform->framebuffer() };

        fb.clear(0xFF335588);
        for(auto& obj : objs) {
            renderer.render(fb, cam, obj);
        }
        platform->present();

        // ---- Frame pacing ----
        const auto frameEnd { clock::now() };
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

    platform->shutdown();
    return 0;
}

