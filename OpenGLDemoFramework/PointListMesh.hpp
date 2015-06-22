#pragma once

#include "Mesh.hpp"
#include <vector>
#include "Vector.hpp"

class PointListMesh : public Mesh
{
public:
	PointListMesh(const std::vector<Vec2>& pointList, const Vec3& color = Vec3(1, 0, 0), const float pointSize = 5.0f);
	~PointListMesh();

	void SetTime(const GLuint time);
	GLuint GetTime() const;

	void SetColor(const Vec3& color);
	Vec3 GetColor() const;

	void SetPointSize(const float color);
	float GetPointSize() const;

	void UseProgram();
	void Render();
private:
	float* genVerts(const std::vector<Vec2>& pointsList);
	GLuint vertexBufferID;
	GLuint colorID;
	Vec3   color;
	GLuint pointSizeID;
	float  pointSize;
	GLuint programID;
	GLuint timeID;
	GLuint time;
};
