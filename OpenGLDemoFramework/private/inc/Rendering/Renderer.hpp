#pragma once

#include <vector>
#include <map>
#include "Math/Vector.hpp"
#include "Core/IScene.hpp"
#include "Core/ICamera.hpp"
#include "Core/ITexture.hpp"
#include "Core/ShaderMaterial.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/FrameBuffer.hpp"
#include "Core/IResourceManager.hpp"
#include "Rendering/Variants/OpenGL/OpenGLResourceManager.hpp"
#include "Core/PerspectiveCamera.hpp"

class Renderer
{
public:
	Renderer(const Vec2& resolution);
	~Renderer();
	void clear(const Vec4& color);
	void setDepthTest(const bool enabled);
    IResourceManager& getResourceManager();
	void render(IScene& scene, ICamera& camera);    
private:
	#ifdef USE_DX12
	#include "../private/inc/Rendering/Variants/RendererDX.hpp"
	#else
	#include "../private/inc/Rendering/Variants/OpenGL/OpenGLRenderer.hpp"
	#endif
};