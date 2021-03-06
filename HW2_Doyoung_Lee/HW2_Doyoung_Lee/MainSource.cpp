/* -------------------------------- 
	References:
	main reference is lecture note provided by Sunghyun Cho, ICE, DGIST.
	"http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-3-matrices/" to make model view matrix
   -------------------------------- */

#include "VertexBufferFunc.h"
#include "UserInputFunc.h"
#include "AnimObjectFunc.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592f

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

int room_row = std::floor(std::rand() % 15) + 10;
int room_col = std::floor(std::rand() % 6) + 4;
int current_window_width = 640;
int current_window_height = 480;

ObjectList room;
Object* player_obj;
glm::vec3 player_destination;

bool alarms[2] = {true, true};

glm::mat4 CreateMVP() {
	
	// Make model view matrix
	glm::vec3 mvp_camera;
	glm::vec3 mvp_focus;
	glm::vec3 mvp_up;
	mvp_camera = mvp_focus = player_obj->GetPosition();
	float mvp_near = 0.1f;
	float mvp_far = 100.0f;
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // glm::perspective is member of matrix_transform.hpp

	switch (modelview_index)
	{
	case 1:
		mvp_camera += glm::vec3(0.25f * std::cos(PI / 2 * move_axis[1]), 0.5f, -0.25f * std::sin(PI / 2 * move_axis[1]));
		mvp_focus += glm::vec3(std::cos(PI / 2 * move_axis[1]), 0.5f, -std::sin(PI / 2 * move_axis[1]));
		mvp_up = { 0, 1, 0 };
		break;
	case 2:
		projection = glm::ortho(-current_window_width / 200.0f, current_window_width / 200.0f, -current_window_height / 200.0f, current_window_height / 200.0f, 0.1f, 100.0f);
		mvp_camera.y = 10;
		//mvp_camera.z = mvp_focus.z = 0;
		mvp_focus.y = 0;
		mvp_up = { 1, 0, 0 };
		break;
	default:
		mvp_camera.x -= 4;
		mvp_camera.y += 3;
		mvp_camera.z = room_col / 2.0;
		mvp_up = { 0, 1, 0 };
		break;
	}

	glm::mat4 view = glm::lookAt(
		mvp_camera,
		mvp_focus,
		mvp_up
	);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = projection * view * model;

	return MVP;
}

void RenderSceneObjCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
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

	GLint matrix_loc = glGetUniformLocation(shader_program, "MVP");
	glm::mat4 MVP = CreateMVP();
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

	// Draw each objects
	glEnable(GL_DEPTH_TEST);
	room.DrawObjects();
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

void AlarmCallBack(int a) {
	alarms[a] = true;
}

void TimerCallBack(int) {
	
	player_obj->SetRotation(glm::vec3(0, move_axis[1] * 0.5 * 3.141592, 0));

	if (alarms[0] && move_axis[0]) {
		glm::vec3 speed = {0.35f, 0.0f, 0.35f};//{ 0.09*std::cos(PI / 2 * move_axis[1])*move_axis[0], 0, -0.09*std::sin(PI / 2 * move_axis[1])*move_axis[0] };
		player_obj->SetVelocity(speed);
		player_destination += glm::vec3(std::cos(PI / 2 * move_axis[1])*move_axis[0], 0, -std::sin(PI / 2 * move_axis[1])*move_axis[0]);
		alarms[0] = false;
		glutTimerFunc(200, AlarmCallBack, 0);
	}

	room.StepObjects();

	glutTimerFunc(33, TimerCallBack, 0);
	glutPostRedisplay();
}

static void InitGlutCallbacks(void) {
	glutDisplayFunc(RenderSceneObjCB);
	glutKeyboardFunc(KeyboardCB);
	glutKeyboardUpFunc(KeyboardUpCB);
	glutSpecialFunc(SpecialCB);
	glutSpecialUpFunc(SpecialUpCB);
	glutTimerFunc(33, TimerCallBack, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(current_window_width, current_window_height);
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
	player_destination = player_obj->GetPosition();
	glutMainLoop();

	return 0;
}