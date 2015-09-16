#pragma once

#include "Math/Vector.hpp"
#include <Core/Texture.hpp>
#include <map>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void clear(const Vec4& color);
	void setDepthTest(const bool enabled);
	int getTexId(const Texture* tex);
private:
	#ifdef USE_DX12
	#include "../private/inc/Rendering/Variants/RendererDX.hpp"
	#else
	#include "../private/inc/Rendering/Variants/GLWrapper.hpp"
	#endif
};