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

int current_window_width = 320, current_window_height = 320;

void InitGlutCallbacks() {

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

	return 0;
}