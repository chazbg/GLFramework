#include "Mesh.hpp"
#include "Vector.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Mesh::Mesh() : 
vertexCount(0),
vertexBuffer(0),
normalsBuffer(0),
wireframeVertexBuffer(0),
showWireframe(false),
castsShadow(false),
receivesShadow(false)
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

	if (0 != texCoordsBuffer)
	{
		delete[] texCoordsBuffer;
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
		vertexSize,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	activateNormalsBuffer();
	activateTexCoordsBuffer();

	if (showWireframe)
	{
		glDrawArrays(GL_LINES, 0, vertexCount * 2);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
	
	glDisableVertexAttribArray(0);

	deactivateNormalsBuffer();
	deactivateTexCoordsBuffer();
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

void Mesh::SetVertexBuffer(const float* vertexBuffer, const unsigned int length, const unsigned char vertexSize)
{
	if (this->vertexBuffer)
	{
		delete[] this->vertexBuffer;
	}

	this->vertexBuffer = new float[length * vertexSize];
	memcpy(this->vertexBuffer, vertexBuffer, length * vertexSize * sizeof(float));
	vertexCount = length;
	this->vertexSize = vertexSize;

	if (vertexSize == 3)
	{
		generateNormals();
		generateWireframe();
	}
	
	
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	if (vertexSize == 3)
	{
		glGenBuffers(1, &normalsBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);

		glGenBuffers(1, &wireframeVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, wireframeVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize * sizeof(float)* 2, wireframeVertexBuffer, GL_STATIC_DRAW);
	}
}

void Mesh::SetNormalsBuffer(const float* normalsBuffer)
{
	if (!this->normalsBuffer)
	{
		delete[] this->normalsBuffer;
	}

	this->normalsBuffer = new float[vertexCount];
	memcpy(this->normalsBuffer, normalsBuffer, vertexCount * sizeof(float));

	glGenBuffers(1, &normalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);
}

void Mesh::SetTexCoordsBuffer(const float* texCoordsBuffer)
{
	if (this->texCoordsBuffer)
	{
		delete[] this->texCoordsBuffer;
	}

	this->texCoordsBuffer = new float[vertexCount * 2];
	memcpy(this->texCoordsBuffer, texCoordsBuffer, vertexCount * 2 * sizeof(float));

	glGenBuffers(1, &texCoordsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 4, texCoordsBuffer, GL_STATIC_DRAW);
}

void Mesh::BindUniform(string uniform)
{
	uniforms[uniform] = glGetUniformLocation(programID, uniform.c_str());
}

void Mesh::SetUniformValue(string uniform, const int v)
{
	glUseProgram(programID);
	glUniform1i(uniforms[uniform], v);
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

void Mesh::SetCastsShadow(const bool castsShadow)
{
	this->castsShadow = castsShadow;
}

void Mesh::SetReceivesShadow(const bool receivesShadow)
{
	this->receivesShadow = receivesShadow;
}

void SetTexture(const Texture& tex)
{
	//TODO
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

void Mesh::activateNormalsBuffer()
{
	if (normalsBuffer)
	{
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
	}
}

void Mesh::activateTexCoordsBuffer()
{

	if (texCoordsBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			BUFFER_OFFSET(0)
			);
	}
}

void Mesh::deactivateNormalsBuffer()
{
	if (normalsBuffer)
	{
		glDisableVertexAttribArray(1);
	}
}

void Mesh::deactivateTexCoordsBuffer()
{
	if (texCoordsBuffer)
	{
		glDisableVertexAttribArray(2);
	}
}