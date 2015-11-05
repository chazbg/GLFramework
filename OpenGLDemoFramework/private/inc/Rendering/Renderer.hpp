#pragma once

#include <Math/Vector.hpp>
#include <Core/IScene.hpp>
#include <Core/ICamera.hpp>
#include <Core/Texture.hpp>
#include <vector>
#include <map>
#include <Core/ShaderMaterial.hpp>
#include <Geometry/Rectangle.hpp>
#include <Core/FrameBuffer.hpp>
#include <Core/TextureFactory.hpp>
#include <Core/PerspectiveCamera.hpp>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void clear(const Vec4& color);
	void setDepthTest(const bool enabled);
	void render(IScene& scene, ICamera& camera);
private:
	#ifdef USE_DX12
	#include "../private/inc/Rendering/Variants/RendererDX.hpp"
	#else
	#include "../private/inc/Rendering/Variants/GLWrapper.hpp"
	#endif
};