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

int room_row = std::floor(std::rand() % 15) + 10;
int room_col = std::floor(std::rand() % 6) + 4;
int current_window_width = 640, current_window_height = 480;

int move_axis[3] = {0, 0, 0}; // front or back (x), left or right (y), rotation
int modelview_index = 0;
bool alarms[2] = { true, true };

Object* player_obj;
Object room(glm::vec3{0.0f, 0.0f, 0.0f}, 0, 0);
glm::vec3 player_destination;

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
		mvp_camera += glm::vec3(0.25f * std::cos(PI / 2 * move_axis[2]), 0.5f, -0.25f * std::sin(PI / 2 * move_axis[2]));
		mvp_focus += glm::vec3(std::cos(PI / 2 * move_axis[2]), 0.5f, -std::sin(PI / 2 * move_axis[2]));
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

void AlarmCallBack(int a) {
	alarms[a] = true;
}

void TimerCallBack(int) {
	player_obj->SetRotation(glm::vec3(0, move_axis[2] * 0.5 * PI, 0));

	if (alarms[0] && move_axis[0]) {
		
		glm::vec3 speed = { 0.35f, 0.0f, 0.0f };//{ 0.09*std::cos(PI / 2 * move_axis[1])*move_axis[0], 0, -0.09*std::sin(PI / 2 * move_axis[1])*move_axis[0] };
		player_obj->SetVelocity(speed);
		player_destination += glm::vec3(move_axis[0], 0, 0);
		alarms[0] = false;
		glutTimerFunc(200, AlarmCallBack, 0);
	}
	if (alarms[0] && move_axis[1]) {
		glm::vec3 speed = { 0.0f, 0.0f, 0.35f };
		player_obj->SetVelocity(speed);
		player_destination += glm::vec3(0, 0, move_axis[1]);
		alarms[0] = false;
		glutTimerFunc(200, AlarmCallBack, 0);
	}

	room.func[Step](&room);

	glutTimerFunc(33, TimerCallBack, 0);
	glutPostRedisplay();
}

void InitGlutCallbacks() {
	glutDisplayFunc(RenderSceneObjCB); // ->VertexBufferFunc.cpp
	glutKeyboardFunc(KeyboardCB); // -> UserInputFunc.cpp
	glutKeyboardUpFunc(KeyboardUpCB); // -> UserInputFunc.cpp
	glutSpecialFunc(SpecialCB); // -> UserInputFunc.cpp
	glutSpecialUpFunc(SpecialUpCB); // -> UserInputFunc.cpp
	glutTimerFunc(33, TimerCallBack, 0);
}

int main(int argc, char** argv) {
	// Initialize GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(current_window_width, current_window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CrossRoad_v3");
	
	// Initialize callback functions
	InitGlutCallbacks();
	
	// GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
		return 1;
	}
	DoInitShader(); // ->VertexBufferFunc.cpp->FileFunc.cpp
	CreateVertexBuffer(); // ->VertexBufferFunc.cpp

	// Create objects
	player_obj = InitObject(&room);
	player_destination = player_obj->GetPosition();

	// Begin openGL main loop
	glutMainLoop();

	return 0;
}