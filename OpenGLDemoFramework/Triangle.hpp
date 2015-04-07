#include "Mesh.hpp"

class Triangle : public Mesh
{
public:
	Triangle();
	~Triangle();

	void SetTime(GLuint time);
	GLuint GetTime();
	
	void SetStartAngle(GLfloat startAngle);
	GLfloat GetStartAngle();

	void SetOffsetAngle(GLfloat offsetAngle);
	GLfloat GetOffsetAngle();

	void UseProgram();
	void Render();
private:
	static const GLfloat g_vertex_buffer_data[];
	GLuint vertexBuffer;
	GLuint programID;
	GLuint timeID;
	GLuint time;
	GLuint startAngleID;
	GLfloat startAngle;
	GLuint offsetAngleID;
	GLfloat offsetAngle;
};
