#include "PlaneGenerator.hpp"

float* generatePlaneVertices(int width, int height)
{
	float* verts = new float[width * height * 6 * 3];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width * 18; j += 18)
		{
			//1
			verts[i * width * 18 + j] = j / 18;
			verts[i * width * 18 + j + 1] = i;
			verts[i * width * 18 + j + 2] = 0;

			//2
			verts[i * width * 18 + j + 3] = j / 18 + 1;
			verts[i * width * 18 + j + 4] = i;
			verts[i * width * 18 + j + 5] = 0;

			//3
			verts[i * width * 18 + j + 6] = j / 18;
			verts[i * width * 18 + j + 7] = i + 1;
			verts[i * width * 18 + j + 8] = 0;

			//4
			verts[i * width * 18 + j + 9] = j / 18 + 1;
			verts[i * width * 18 + j + 10] = i;
			verts[i * width * 18 + j + 11] = 0;

			//5
			verts[i * width * 18 + j + 12] = j / 18 + 1;
			verts[i * width * 18 + j + 13] = i + 1;
			verts[i * width * 18 + j + 14] = 0;

			//6
			verts[i * width * 18 + j + 15] = j / 18;
			verts[i * width * 18 + j + 16] = i + 1;
			verts[i * width * 18 + j + 17] = 0;
		}
	}
	return verts;
}