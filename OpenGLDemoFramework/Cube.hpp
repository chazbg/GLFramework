#include "Mesh.hpp"

class Cube : public Mesh
{
public:
	Cube();
	~Cube();

	void SetTime(GLuint time);
	GLuint GetTime();

	void UseProgram();
	void Render();
	void GenerateNormals();
private:
	static const GLfloat vertices[];
	GLuint vertexBuffer;
	GLuint normalsBuffer;
	GLuint programID;
	GLuint timeID;
	GLuint time;
	GLfloat* buffer;
};
