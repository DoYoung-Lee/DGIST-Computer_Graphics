/* -------------------------------- 
	References:
	main reference is lecture note provided by Sunghyun Cho, ICE, DGIST.
   -------------------------------- */

#include "VertexBufferFunc.h"
#include "UserInputFunc.h"

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

std::vector<glm::vec3> vertices_obj;
std::vector<glm::vec3> colors_obj;
std::vector<unsigned int> indices_obj;

bool alarms[2] = {true, true};

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