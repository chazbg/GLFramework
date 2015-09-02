#pragma once

#include "Math/Vector.hpp"
#include "Core/IScene.hpp"
#include "Core/ICamera.hpp"
#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void clear(const Vec4& color);
	void setDepthTest(const bool enabled);
	void render(IScene& scene, ICamera& camera);
private:
	void render(std::vector<IMesh*>& meshes, ICamera& camera);
	void render(IMesh* mesh, ICamera& camera);
	#ifdef USE_DX12
	#include "../private/inc/Rendering/Variants/RendererDX.hpp"
	#else
	#include "../private/inc/Rendering/Variants/GLWrapper.hpp"
	#endif
};