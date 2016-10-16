#include "Demos/DemoGForceMeter.hpp"

Vec2 GForceMeterDemo::TestWindowApp::interpolateP(Vec2 p0, Vec2 m0, Vec2 p1, Vec2 m1, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    return (2.0f  * t3   - 3.0f * t2 + 1.0f) * p0 +
           (t3    - 2.0f * t2   + t)         * m0 +
           (-2.0f * t3   + 3.0f * t2)        * p1 +
           (t3    - t2)                      * m1;
}

Vec2 GForceMeterDemo::TestWindowApp::interpolateM(Vec2 p0, Vec2 m0, Vec2 p1, Vec2 m1, float t)
{
    float t2 = t * t;

    return (6.0f  * t2 - 6.0f * t)        * p0 +
           (3.0f  * t2 - 4.0f * t + 1.0f) * m0 +
           (-6.0f * t2 + 6.0f * t)        * p1 +
           (3.0f  * t2 - 2.0f * t)        * m1;
}

void GForceMeterDemo::main()
{
    WindowParameters params;
    params.width = 800;
    params.height = 800;
    params.posX = 100;
    params.posY = 100;
    params.name = "TestWindow";
    TestWindowApp app(Vec2(static_cast<float>(params.width),
        static_cast<float>(params.height)));
    Window window(params, app);
    window.startRenderLoop();
}
