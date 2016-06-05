#pragma once

#include "Rendering/Variants/OpenGL/OpenGLRenderer.hpp"

class Renderer : public OpenGLRenderer
{
public:
    Renderer(Vec2 resolution) : OpenGLRenderer(resolution) {}
};