#include "Mesh.hpp"
#include "Vector.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Mesh::Mesh() : 
vertexCount(0),
vertexBuffer(0),
normalsBuffer(0),
wireframeVertexBuffer(0),
showWireframe(false)
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

void Mesh::Render()
{
	glUseProgram(programID);

	if (showWireframe)
	{
		glBindBuffer(GL_ARRAY_BUFFER, wireframeVertexBufferID);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	}
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	if (showWireframe)
	{
		glDrawArrays(GL_LINES, 0, vertexCount * 2);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Mesh::SetProjectionMatrix(const Matrix4& projection)
{
	this->projection = projection;
	SetUniformValue("mvp", projection * view * model);
}

void Mesh::SetViewMatrix(const Matrix4& view)
{
	this->view = view;
	SetUniformValue("mvp", projection * view * model);
}

void Mesh::SetModelMatrix(const Matrix4& model)
{
	this->model = model;
	SetUniformValue("mvp", projection * view * model);
}

void Mesh::SetShaders(const string vertexShaderPath, const string fragmentShaderPath)
{
	programID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	BindUniform("mvp");
	SetUniformValue("mvp", projection * view * model);
}

void Mesh::SetWireframeMode(const bool showWireframe)
{
	this->showWireframe = showWireframe;
}

void Mesh::SetVertexBuffer(const float* vertexBuffer, const unsigned int length)
{
	if (this->vertexBuffer)
	{
		delete[] this->vertexBuffer;
	}

	this->vertexBuffer = new float[length * 3];
	memcpy(this->vertexBuffer, vertexBuffer, length * 3 * sizeof(float));
	vertexCount = length;

	generateNormals();
	generateWireframe();

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &wireframeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, wireframeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4 * 2, wireframeVertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);
}

void Mesh::SetNormalsBuffer(const float* normalsBuffer, const unsigned int length)
{
	if (!this->normalsBuffer)
	{
		delete[] this->normalsBuffer;
	}

	this->normalsBuffer = new float[length];
	memcpy(this->normalsBuffer, normalsBuffer, length * sizeof(float));

	glGenBuffers(1, &normalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);
}

void Mesh::BindUniform(string uniform)
{
	uniforms[uniform] = glGetUniformLocation(programID, uniform.c_str());
}

void Mesh::SetUniformValue(string uniform, const unsigned int v)
{
	glUseProgram(programID);
	glUniform1ui(uniforms[uniform], v);
}

void Mesh::SetUniformValue(string uniform, const Vec3& v)
{
	//TODO
}

void Mesh::SetUniformValue(string uniform, const Vec4& v)
{
	//TODO
}

void Mesh::SetUniformValue(string uniform, const Matrix4& v)
{
	glUseProgram(programID);
	glUniformMatrix4fv(uniforms[uniform], 1, false, v.raw());
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

void Mesh::generateWireframe()
{
	wireframeVertexBuffer = new float[vertexCount * 2];
	unsigned int j = 0;
	for (unsigned int i = 0; i < vertexCount; i += 9)
	{
		wireframeVertexBuffer[j] = vertexBuffer[i];
		wireframeVertexBuffer[j + 1] = vertexBuffer[i + 1];
		wireframeVertexBuffer[j + 2] = vertexBuffer[i + 2];

		wireframeVertexBuffer[j + 3] = vertexBuffer[i + 3];
		wireframeVertexBuffer[j + 4] = vertexBuffer[i + 4];
		wireframeVertexBuffer[j + 5] = vertexBuffer[i + 5];

		wireframeVertexBuffer[j + 6] = vertexBuffer[i + 3];
		wireframeVertexBuffer[j + 7] = vertexBuffer[i + 4];
		wireframeVertexBuffer[j + 8] = vertexBuffer[i + 5];

		wireframeVertexBuffer[j + 9] = vertexBuffer[i + 6];
		wireframeVertexBuffer[j + 10] = vertexBuffer[i + 7];
		wireframeVertexBuffer[j + 11] = vertexBuffer[i + 8];

		wireframeVertexBuffer[j + 12] = vertexBuffer[i + 6];
		wireframeVertexBuffer[j + 13] = vertexBuffer[i + 7];
		wireframeVertexBuffer[j + 14] = vertexBuffer[i + 8];

		wireframeVertexBuffer[j + 15] = vertexBuffer[i];
		wireframeVertexBuffer[j + 16] = vertexBuffer[i + 1];
		wireframeVertexBuffer[j + 17] = vertexBuffer[i + 2];
		j += 18;
	}
}