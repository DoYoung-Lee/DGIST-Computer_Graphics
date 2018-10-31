/* -------------------------------- 
	References:
	main reference is lecture note provided by Sunghyun Cho, ICE, DGIST.
   -------------------------------- */

#include "UserInputFunc.h"
#include "VertexBufferFunc.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint shader_program;
bool alarms[2] = {true, true};

static void RenderScene2DCB() { // legacy code for 2d drawing
	glClear(GL_COLOR_BUFFER_BIT);

	GLint position_loc = glGetAttribLocation(shader_program, "Position");
	GLint color_loc = glGetAttribLocation(shader_program, "Color");

	GLint x_loc, y_loc, r_loc;
	x_loc = glGetUniformLocation(shader_program, "x");
	y_loc = glGetUniformLocation(shader_program, "y");
	r_loc = glGetUniformLocation(shader_program, "rot");

	glUniform1f(x_loc, 0);
	glUniform1f(y_loc, 0);
	glUniform1f(r_loc, 0);

	//// Draw background
	// 1st attribute buffer: vertices
	glEnableVertexAttribArray(position_loc);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 2nd attribute buffer: colors
	glEnableVertexAttribArray(color_loc);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

static void RenderScene3DCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make model view matrix
	GLint matrix_loc = glGetUniformLocation(shader_program, "MVP");
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // glm::perspective is member of matrix_transform.hpp
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, -3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

	// 1st attribute buffer: vertices
	GLint position_loc = glGetAttribLocation(shader_program, "Position");
	glEnableVertexAttribArray(position_loc);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 2nd attribute buffer: colors
	GLint color_loc = glGetAttribLocation(shader_program, "Color");
	glEnableVertexAttribArray(color_loc);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Scale object
	GLint scale_loc = glGetUniformLocation(shader_program, "Scale");
	glUniform1f(scale_loc, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

void TimerCallBack(int) {

	glutTimerFunc(33, TimerCallBack, 0);
	glutPostRedisplay();
}

void AlarmCallBack(int a) {
	alarms[a] = true;
}

static void InitGlutCallbacks(void) {
	glutDisplayFunc(RenderScene3DCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialCB);
	glutSpecialUpFunc(SpecialUpCB);
	glutTimerFunc(33, TimerCallBack, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CrossRoad3D");
	InitGlutCallbacks(); // Initialize callback functions
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
		return 1;
	}
	DoInitShader(&shader_program);
	CreateVertexBuffer(); // Create vertex buffer using vertice data
	glutMainLoop();

	return 0;
}