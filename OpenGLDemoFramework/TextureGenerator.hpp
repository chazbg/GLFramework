#pragma once
#include <vector>
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

		int c1 = 0xFF00FFFF;
		int c2 = 0xFF0000FF;
		int i = 0;
		float t = 0.0f;
		float delta = (float)1 / (float)(SIZE / 2 - 1);
		for (i = 0, t = 0.0f; i < SIZE / 2, t <= 1.0f; i++, t += delta)
		{
			tex[i] = (int)(c1 * (1 - t)) + (int)(c2 * t);
		}

		for (i = SIZE / 2, t = 0.0f; i < SIZE, t <= 1.0f; i++, t += delta)
		{
			tex[i] = (int)(c2 * (1 - t)) + (int)(c1 * t);
		}

		return tex;
	}
private:
	std::vector<unsigned int*> textures;
};