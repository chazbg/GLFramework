#include "Mesh.hpp"
#include "Vector.hpp"

Mesh::Mesh() : 
vertexCount(0),
vertexBuffer(0),
normalsBuffer(0),
wireframeVertexBuffer(0)
{
}

Mesh::~Mesh()
{
	if (0 != vertexBuffer)
	{
		delete[] vertexBuffer;
	}

	if (0 != normalsBuffer)
	{
		delete[] normalsBuffer;
	}

	if (0 != wireframeVertexBuffer)
	{
		delete[] wireframeVertexBuffer;
	}
}

void Mesh::SetViewMatrix(const Matrix4& view)
{
	this->view = view;
}

void Mesh::generateNormals()
{
	normalsBuffer = new float[vertexCount * 3];

	for (unsigned int i = 0; i < vertexCount * 3; i += 9)
	{
		Vec3 a(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2]);
		Vec3 b(vertexBuffer[i + 3], vertexBuffer[i + 4], vertexBuffer[i + 5]);
		Vec3 c(vertexBuffer[i + 6], vertexBuffer[i + 7], vertexBuffer[i + 8]);
		Vec3 cb = c - b;
		Vec3 ab = a - b;
		Vec3 res = (cb * ab).normalize();

		normalsBuffer[i] = res.x;
		normalsBuffer[i + 1] = res.y;
		normalsBuffer[i + 2] = res.z;

		normalsBuffer[i + 3] = res.x;
		normalsBuffer[i + 4] = res.y;
		normalsBuffer[i + 5] = res.z;

		normalsBuffer[i + 6] = res.x;
		normalsBuffer[i + 7] = res.y;
		normalsBuffer[i + 8] = res.z;
	}
}