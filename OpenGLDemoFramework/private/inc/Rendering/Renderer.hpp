#pragma once

#include "Math/Vector.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();
	void clear(const Vec4& color);
private:
	#ifdef USE_DX12
	#include "../private/inc/Rendering/Variants/RendererDX.hpp"
	#else
	#include "../private/inc/Rendering/Variants/GLWrapper.hpp"
	#endif
};