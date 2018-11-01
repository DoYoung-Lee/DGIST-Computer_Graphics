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
GLuint indexbuffer_obj;

std::vector<glm::vec3> vertices_obj;
std::vector<glm::vec3> colors_obj;
std::vector<unsigned int> indices_obj;

int move_axis[2] = { 0, 0 };
int modelview_index = 0;

ObjectList room;
Object* player_obj;

bool alarms[2] = {true, true};

void CreateMVP() {
	// Make model view matrix
	GLint matrix_loc = glGetUniformLocation(shader_program, "MVP");

	glm::vec3 mvp_camera;
	glm::vec3 mvp_focus;
	glm::vec3 mvp_up;
	float mvp_near;
	float mvp_far;

	switch (modelview_index)
	{
	case 1:
		mvp_camera = player_obj->GetPosition();
		mvp_focus = {mvp_camera.x + 1, mvp_camera.y, mvp_camera.z}; // Specific coordination is not calculated.
		mvp_up = { mvp_camera.x, mvp_camera.y + 1, mvp_camera.z };
		mvp_near = 0.1f;
		mvp_far = 100.0f;
		break;
	case 2:
		mvp_camera = { 0, 10, 0 };
		mvp_focus = { 0, 0, 0 };
		mvp_up = { 1, 0, 0 };
		mvp_near = 0.1f;
		mvp_far = 100.0f;
		break;
	default:
		mvp_camera = { 4, 4, -3 };
		mvp_focus = { 0, 0, 0 };
		mvp_up = { 0, 1, 0 };
		mvp_near = 0.1f;
		mvp_far = 100.0f;
		break;
	}

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, mvp_near, mvp_far); // glm::perspective is member of matrix_transform.hpp
	glm::mat4 view = glm::lookAt(
		mvp_camera,
		mvp_focus,
		mvp_up
	);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = projection * view * model;

	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);
}

void RenderSceneObjCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CreateMVP();
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer_obj);

	// Draw each objects
	glEnable(GL_DEPTH_TEST);
	room.DrawObjects();
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

void TimerCallBack(int) {
	// Update speed of player
	
	glm::vec3 speed = { 0.1*move_axis[0], 0, 0.1*move_axis[1]};
	player_obj->SetVelocity(speed);

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
	player_obj = InitObject(&room);
	glutMainLoop();

	return 0;
}