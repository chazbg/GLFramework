#pragma once

#include "Mesh.hpp"
#include <vector>
#include "Vector.hpp"

class LineListMesh : public Mesh
{
public:
	LineListMesh(const std::vector<Vec2>& pointList, const Vec3& color = Vec3(1, 0, 0), const float lineSize = 5.0f);
	LineListMesh(const std::vector<Vec3>& pointList, const Vec3& color = Vec3(1, 0, 0), const float lineSize = 5.0f);
	~LineListMesh();

	void SetTime(const GLuint time);
	GLuint GetTime() const;

	void SetColor(const Vec3& color);
	Vec3 GetColor() const;

	void SetLineSize(const float size);
	float GetLineSize() const;

	void UseProgram();
	void Render();
private:
	float* genVerts(const std::vector<Vec2>& pointsList);
	float* genVerts(const std::vector<Vec3>& pointsList);
	GLuint vertexBufferID;
	GLuint colorID;
	Vec3   color;
	GLuint lineSizeID;
	float  lineSize;
	GLuint programID;
	GLuint timeID;
	GLuint time;
	bool is2d;
};
