#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/Shader.hpp"
#include <algorithm>
#include <cstdio>
#include <Core/VertexBufferObject.hpp>

#define BUFFER_OFFSET(i) ((void*)(i))

Mesh::Mesh() : 
material(0),
wireframeVertexBuffer(0),
showWireframe(false),
castsShadow(false),
receivesShadow(false)
{
	for (int i = 0; i < 3; i++)
	{
		vbos.push_back(new VertexBufferObject());
	}
}

Mesh::~Mesh()
{
	delete[] wireframeVertexBuffer;
}

std::vector<IVertexBufferObject*>& Mesh::getVBOs()
{
	return vbos;
}

void Mesh::setProjectionMatrix(const Matrix4 & projection)
{
	this->projection = projection;
}

void Mesh::setViewMatrix(const Matrix4 & view)
{
	this->view = view;
}

void Mesh::setModelMatrix(const Matrix4 & model)
{
	this->model = model;
}

Matrix4 Mesh::getProjectionMatrix() const
{
	return projection;
}

Matrix4 Mesh::getViewMatrix() const
{
	return view;
}

Matrix4 Mesh::getModelMatrix() const
{
	return model;
}

void Mesh::setWireframeMode(const bool showWireframe)
{
	this->showWireframe = showWireframe;
}

void Mesh::setVertices(const IVertexBufferObject & vertices)
{
	vbos[0] = &const_cast<IVertexBufferObject&>(vertices);
}

void Mesh::setNormals(const IVertexBufferObject & normals)
{
	vbos[1] = &const_cast<IVertexBufferObject&>(normals);
}

void Mesh::setTexCoords(const IVertexBufferObject & texCoords)
{
	vbos[2] = &const_cast<IVertexBufferObject&>(texCoords);
}

void Mesh::setMaterial(IMaterial * material)
{
	this->material = material;
}

IMaterial & Mesh::getMaterial() const
{
	return *material;
}

void Mesh::SetUniformValue(string uniform, const int v)
{
    material->setProperty(uniform, v);
}

void Mesh::SetUniformValue(string uniform, const unsigned int v)
{
    material->setProperty(uniform, v);
}

void Mesh::SetUniformValue(string uniform, const Vec3& v)
{
    material->setProperty(uniform, v);
}

void Mesh::SetUniformValue(string uniform, const Vec4& v)
{
	//TODO
}

void Mesh::SetUniformValue(string uniform, const Matrix4& v)
{
    material->setProperty(uniform, v);
}

void Mesh::SetCastsShadow(const bool castsShadow)
{
	this->castsShadow = castsShadow;
}

void Mesh::SetReceivesShadow(const bool receivesShadow)
{
	this->receivesShadow = receivesShadow;
}

void Mesh::SetPosition(const Vec3& position)
{
	model.setTranslation(position);
	SetUniformValue("mvp", projection * view * model);
}

float* Mesh::generateNormals(const float* vertexBuffer, const unsigned int vertexCount)
{
	float* normalsBuffer = new float[vertexCount * 3];

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

    return normalsBuffer;
}

void Mesh::generateWireframe()
{
	//wireframeVertexBuffer = new float[vertexCount * 3 * 2];
	//unsigned int j = 0;
	//for (unsigned int i = 0; i < vertexCount; i += 9)
	//{
	//	wireframeVertexBuffer[j] = vertexBuffer[i];
	//	wireframeVertexBuffer[j + 1] = vertexBuffer[i + 1];
	//	wireframeVertexBuffer[j + 2] = vertexBuffer[i + 2];

	//	wireframeVertexBuffer[j + 3] = vertexBuffer[i + 3];
	//	wireframeVertexBuffer[j + 4] = vertexBuffer[i + 4];
	//	wireframeVertexBuffer[j + 5] = vertexBuffer[i + 5];

	//	wireframeVertexBuffer[j + 6] = vertexBuffer[i + 3];
	//	wireframeVertexBuffer[j + 7] = vertexBuffer[i + 4];
	//	wireframeVertexBuffer[j + 8] = vertexBuffer[i + 5];

	//	wireframeVertexBuffer[j + 9] = vertexBuffer[i + 6];
	//	wireframeVertexBuffer[j + 10] = vertexBuffer[i + 7];
	//	wireframeVertexBuffer[j + 11] = vertexBuffer[i + 8];

	//	wireframeVertexBuffer[j + 12] = vertexBuffer[i + 6];
	//	wireframeVertexBuffer[j + 13] = vertexBuffer[i + 7];
	//	wireframeVertexBuffer[j + 14] = vertexBuffer[i + 8];

	//	wireframeVertexBuffer[j + 15] = vertexBuffer[i];
	//	wireframeVertexBuffer[j + 16] = vertexBuffer[i + 1];
	//	wireframeVertexBuffer[j + 17] = vertexBuffer[i + 2];
	//	j += 18;
	//}
}

void Mesh::activateNormalsBuffer()
{
	//if (normalsBuffer)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(
	//		1,
	//		3,
	//		GL_FLOAT,
	//		GL_FALSE,
	//		0,
	//		BUFFER_OFFSET(0)
	//		);
	//}
}

void Mesh::activateTexCoordsBuffer()
{

	//if (texCoordsBuffer)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(
	//		2,
	//		2,
	//		GL_FLOAT,
	//		GL_FALSE,
	//		0,
	//		BUFFER_OFFSET(0)
	//		);
	//}
}

void Mesh::deactivateNormalsBuffer()
{
	//if (normalsBuffer)
	//{
	//	glDisableVertexAttribArray(1);
	//}
}

void Mesh::deactivateTexCoordsBuffer()
{
	//if (texCoordsBuffer)
	//{
	//	glDisableVertexAttribArray(2);
	//}
}