#pragma once

#include <vector>
#include <map>
#include <functional>
#include "Math/Vector.hpp"
#include "Core/IScene.hpp"
#include "Core/ICamera.hpp"
#include "Core/ITexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLMaterial.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/FrameBuffer.hpp"
#include "Core/IResourceManager.hpp"
#include "Rendering/Variants/OpenGL/OpenGLResourceManager.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Rendering/Variants/OpenGL/OpenGLRenderer.hpp"

class Renderer : public OpenGLRenderer
{
public:
    Renderer(Vec2 resolution) : OpenGLRenderer(resolution) {}
};