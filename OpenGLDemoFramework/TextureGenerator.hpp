#pragma once
#include <vector>
#include "Vector.hpp"
#include <cstdio>
#define SIZE 256
class TextureGenerator
{
public:
	TextureGenerator() {}
	~TextureGenerator() 
	{ 
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			delete textures[i];
		}
	}

	unsigned int* generateGradient()
	{
		unsigned int* tex = new unsigned int[SIZE];
		textures.push_back(tex);
		Vec3 c1(0.0f, 0.0f, 0.0f);
		Vec3 c2(1.0f, 0.0f, 0.0f);
		int i = 0;
		float t = 0.0f;
		float delta = (float) 1 / (float) (SIZE / 2 - 1);

		for (i = 0, t = 0.0f; i < SIZE / 2, t <= 1.0f; i++, t += delta)
		{
			Vec3 resColor = (c1 * (1 - t)) + (c2 * t);
			tex[i] = toABGR(resColor);
		}

		for (i = SIZE / 2, t = 0.0f; i < SIZE, t <= 1.0f; i++, t += delta)
		{
			Vec3 resColor = (c2 * (1 - t)) + (c1 * t);
			tex[i] = toABGR(resColor);
		}

		return tex;
	}
private:
	unsigned int toABGR(const Vec3 color)
	{
		unsigned int res = 0xFF000000;
		res += (int) (color.x * 255) + ((int) (color.y * 255) << 8) + ((int) (color.z * 255) << 16);
		return res;
	}

	std::vector<unsigned int*> textures;
};