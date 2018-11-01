/* -------------------------------- 
	References:
	main reference is lecture note provided by Sunghyun Cho, ICE, DGIST.
   -------------------------------- */

#include "VertexBufferFunc.h"
#include "UserInputFunc.h"
#include "AnimObjectFunc.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint shader_program;

GLuint vertex_array_obj;
GLuint vertexbuffer_obj;
GLuint colorbuffer_obj;
GLuint indexbufer_obj;

ObjectList room;

std::vector<glm::vec3> vertices_obj;
std::vector<glm::vec3> colors_obj;
std::vector<unsigned int> indices_obj;

bool alarms[2] = {true, true};

void InitObject() {
	// fill grass

	// create tree on the road

	// fill road

	// create car on the road

	// build wall

	// create player marker
	Object* player = new Object;
	room.CreateObject(player);
	player->SetPosition({ 0, 0, 0 });
	player->SetVelocity({ 0, 0, 0 });
	player->SetModel(12, 0);

	Object* player2 = new Object;
	room.CreateObject(player2);
	player2->SetPosition({ 0.5, 0.5, 0.5 });
	player2->SetVelocity({ 0.1, 0, 0 });
	player2->SetModel(12, 0);
}

void RenderSceneObjCB() {
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
	glUniform1f(scale_loc, 0.5f);

	glEnable(GL_DEPTH_TEST);
	room.DrawObjects();
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

void TimerCallBack(int) {
	room.StepObjects();

	glutTimerFunc(33, TimerCallBack, 0);
	glutPostRedisplay();
}

void AlarmCallBack(int a) {
	alarms[a] = true;
}

static void InitGlutCallbacks(void) {
	glutDisplayFunc(RenderSceneObjCB);
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
	InitObject();
	glutMainLoop();

	return 0;
}