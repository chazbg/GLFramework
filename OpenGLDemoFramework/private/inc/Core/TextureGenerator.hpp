#pragma once
#include <vector>
#include "Math/Vector.hpp"
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
	unsigned int* generatePerlinNoise()
	{
		static int permutation[] = { 151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,  
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };


		int p[SIZE * 2];
		for (int x = 0; x < SIZE * 2; x++) {
			p[x] = permutation[x % SIZE];
		}

		unsigned char*  tex = new unsigned char[SIZE*SIZE*4];
		//textures.push_back(tex);

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE * 4; j+=4)
			{
				float v = 255.0f * perlin(j / 256.0f, i / 64.0f, 0, p);
				tex[i * SIZE * 4 + j] = v;
				tex[i * SIZE * 4 + j + 1] = v;
				tex[i * SIZE * 4 + j + 2] = v;
				tex[i * SIZE * 4 + j + 3] = 255;
			}
		}

		return (unsigned int*) tex;
	}

private:
	unsigned int toABGR(const Vec3 color)
	{
		unsigned int res = 0xFF000000;
		res += (int) (color.x * 255) + ((int) (color.y * 255) << 8) + ((int) (color.z * 255) << 16);
		return res;
	}

	float splineInterpolate(float t)
	{
		return t * t * t * (t * t * 6.0f - t * 15.0f + 10.0f);
	}

	float linearInterpolate(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	float grad(int hash, float x, float y, float z)
	{
		switch (hash & 0xF)
		{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0; // never happens
		}
	}

	int inc(int x)
	{
		return x+1;
	}

	float perlin(float x, float y, float z, int* p)
	{
		int xi = (int)x & 255;
		int yi = (int)y & 255;
		int zi = (int)z & 255;
		float xf = x - (int)x;
		float yf = y - (int)y;
		float zf = z - (int)z;

		float u = splineInterpolate(xf);
		float v = splineInterpolate(yf);
		float w = splineInterpolate(zf);

		int aaa, aba, aab, abb, baa, bba, bab, bbb;
		aaa = p[p[p[xi] + yi] + zi];
		aba = p[p[p[xi] + inc(yi)] + zi];
		aab = p[p[p[xi] + yi] + inc(zi)];
		abb = p[p[p[xi] + inc(yi)] + inc(zi)];
		baa = p[p[p[inc(xi)] + yi] + zi];
		bba = p[p[p[inc(xi)] + inc(yi)] + zi];
		bab = p[p[p[inc(xi)] + yi] + inc(zi)];
		bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

		float x1, x2, y1, y2;
		x1 = linearInterpolate(grad(aaa, xf, yf, zf),           // The gradient function calculates the dot product between a pseudorandom
			grad(baa, xf - 1, yf, zf),             // gradient vector and the vector from the input coordinate to the 8
			u);                                     // surrounding points in its unit cube.
		x2 = linearInterpolate(grad(aba, xf, yf - 1, zf),           // This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
			grad(bba, xf - 1, yf - 1, zf),             // values we made earlier.
			u);
		y1 = linearInterpolate(x1, x2, v);

		x1 = linearInterpolate(grad(aab, xf, yf, zf - 1),
			grad(bab, xf - 1, yf, zf - 1),
			u);
		x2 = linearInterpolate(grad(abb, xf, yf - 1, zf - 1),
			grad(bbb, xf - 1, yf - 1, zf - 1),
			u);
		y2 = linearInterpolate(x1, x2, v);

		return (linearInterpolate(y1, y2, w) + 1) / 2;
	}
	std::vector<unsigned int*> textures;
};