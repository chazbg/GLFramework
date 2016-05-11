#pragma once

#include "Core/Mesh.hpp"
#include <vector>
#include "Math/Vector.hpp"

class PointListMesh : public Mesh
{
public:
    PointListMesh(const std::vector<Vec2>& pointList, const Vec3& color = Vec3(1, 0, 0), const float pointSize = 5.0f);
    PointListMesh(const std::vector<Vec3>& pointList, const Vec3& color = Vec3(1, 0, 0), const float pointSize = 5.0f);
    ~PointListMesh();

    void SetTime(const GLuint time);
    GLuint GetTime() const;

    void SetColor(const Vec3& color);
    Vec3 GetColor() const;

    void SetPointSize(const float size);
    float GetPointSize() const;

    void UseProgram();
    void Render();
private:
    float* genVerts(const std::vector<Vec2>& pointsList);
    float* genVerts(const std::vector<Vec3>& pointsList);
    GLuint vertexBufferID;
    GLuint colorID;
    Vec3   color;
    GLuint pointSizeID;
    float  pointSize;
    GLuint programID;
    GLuint timeID;
    GLuint time;
    bool   is2d;
};
