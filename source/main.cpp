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

    std::unique_ptr<IPlatform> platform { std::make_unique<SDLPlatform>() }; 

    if (!platform->initialize(800, 600, "Software Renderer")) {
        return -1;
    }
    platform->registerWireframeToggleCallback([&](){
        renderer.toggleWireframe();
    });
    platform->registerForwardCallback([&](double dt){
        auto fwd = renderer.getCamera().forward();
        fwd *= {dt,dt,dt};
        renderer.getCamera().position += fwd;
    });
    platform->registerBackwardCallback([&](double dt){
        auto bwd = renderer.getCamera().forward();
        bwd *= {-dt,-dt,-dt};
        renderer.getCamera().position += bwd;
    });
    platform->registerRightCallback([&](double dt){
        auto r = renderer.getCamera().right();
        r *= {-dt,-dt,-dt};
        renderer.getCamera().position += r;
    });
    platform->registerLeftCallback([&](double dt){
        auto l = renderer.getCamera().right();
        l *= {dt,dt,dt};
        renderer.getCamera().position += l;
    });
    platform->registerCameraUpdateCallback([&](double dt, int dx, int dy){
        renderer.getCamera().updateCamera(dt, dx, dy);
    });

    using clock = std::chrono::steady_clock;

    auto lastTime{ clock::now() };
    auto running { true };

        renderer.toggleWireframe();
    while (running)
    {
        auto frameStart = clock::now();

        std::chrono::duration<double> delta { frameStart - lastTime };
        lastTime = frameStart;

        auto deltaTime { delta.count() };

        if (deltaTime > 0.25)
            // Clamp deltaTime (prevents huge jumps if debugger pauses)
            deltaTime = 0.25;

        running = platform->processEvents(deltaTime);

        rotateObj(objs[0], M_PI * deltaTime / 10);

        auto& fb { platform->framebuffer() };
        fb.clear(0xFF335588);
        for(auto& obj : objs) {
            renderer.render(fb, obj);
        }
        platform->present();

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

