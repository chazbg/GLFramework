#include "Geometry/PlaneMesh.hpp"

PlaneMesh::PlaneMesh(IResourceManager& rm, int width, int height) :
    Mesh(),
    rm(rm),
    width(width),
    height(height)
{
    unsigned int vertexCount = width * height * 6;
    float* vb = generatePlaneVertices(width, height);
    float* uvBuffer = generateUVs(width, height);

    vertices = rm.createVertexBuffer(vertexCount, 3, 0, vb);
    uvs      = rm.createVertexBuffer(vertexCount, 2, 1, uvBuffer);

    setVertices(*vertices);
    setTexCoords(*uvs);

    delete[] vb;
    delete[] uvBuffer;
}

PlaneMesh::~PlaneMesh()
{
    rm.destroyVertexBuffer(vertices);
    rm.destroyVertexBuffer(uvs);
}

float* PlaneMesh::generatePlaneVertices(int width, int height)
{
    float* verts = new float[width * height * 6 * 3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 18; j += 18)
        {
            //1
            verts[i * width * 18 + j] = j / 18.0f;
            verts[i * width * 18 + j + 1] = (float) i;
            verts[i * width * 18 + j + 2] = 0.0f;

            //2
            verts[i * width * 18 + j + 3] = j / 18.0f + 1.0f;
            verts[i * width * 18 + j + 4] = (float) i;
            verts[i * width * 18 + j + 5] = 0.0f;

            //3
            verts[i * width * 18 + j + 6] = j / 18.0f;
            verts[i * width * 18 + j + 7] = (float) i + 1.0f;
            verts[i * width * 18 + j + 8] = 0.0f;

            //4
            verts[i * width * 18 + j + 9] = j / 18.0f + 1.0f;
            verts[i * width * 18 + j + 10] = (float) i;
            verts[i * width * 18 + j + 11] = 0.0f;

            //5
            verts[i * width * 18 + j + 12] = j / 18.0f + 1.0f;
            verts[i * width * 18 + j + 13] = (float) i + 1.0f;
            verts[i * width * 18 + j + 14] = 0.0f;

            //6
            verts[i * width * 18 + j + 15] = j / 18.0f;
            verts[i * width * 18 + j + 16] = (float) i + 1.0f;
            verts[i * width * 18 + j + 17] = 0.0f;
        }
    }
    return verts;
}

float * PlaneMesh::generateUVs(int width, int height)
{
    float* uvs = new float[width * height * 6 * 2];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = 0; j < width * 18; j += 18, k += 12)
        {
            //1
            uvs[i * width * 12 + k] = (k / 12) / (float) width;
            uvs[i * width * 12 + k + 1] = i / (float) height;

            //2
            uvs[i * width * 12 + k + 2] = ((k / 12) + 1) / (float) width;
            uvs[i * width * 12 + k + 3] = i / (float) height;

            //3
            uvs[i * width * 12 + k + 4] = (k / 12) / (float) width;
            uvs[i * width * 12 + k + 5] = (i + 1) / (float) height;

            //4
            uvs[i * width * 12 + k + 6] = ((k / 12) + 1) / (float) width;
            uvs[i * width * 12 + k + 7] = i / (float) height;

            //5
            uvs[i * width * 12 + k + 8] = ((k / 12) + 1) / (float) width;
            uvs[i * width * 12 + k + 9] = (i + 1) / (float) height;

            //6
            uvs[i * width * 12 + k + 10] = (k / 12) / (float) width;
            uvs[i * width * 12 + k + 11] = (i + 1) / (float) height;
        }
    }
    return uvs;
}
