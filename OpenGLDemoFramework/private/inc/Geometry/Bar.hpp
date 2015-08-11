#include "Mesh.hpp"

class Bar : public Mesh
{
public:
	Bar();
	~Bar();

	void SetTime(GLuint time);
	GLuint GetTime();

	void SetSegments(GLuint segments);
	GLuint GetSegments();

	void SetStartAngle(GLfloat startAngle);
	GLfloat GetStartAngle();

	void SetOffsetAngle(GLfloat offsetAngle);
	GLfloat GetOffsetAngle();

	void SetFillPercentage(GLfloat fillPercentage);
	GLfloat GetFillPercentage();

	void UseProgram();
	void Render();
private:
	static const GLfloat g_vertex_buffer_data[];
	GLuint vertexBuffer;
	GLuint programID;
	GLuint timeID;
	GLuint time;
	GLuint segmentsID;
	GLuint segments;
	GLuint startAngleID;
	GLfloat startAngle;
	GLuint offsetAngleID;
	GLfloat offsetAngle;
	GLuint fillPercentageID;
	GLfloat fillPercentage;
};
