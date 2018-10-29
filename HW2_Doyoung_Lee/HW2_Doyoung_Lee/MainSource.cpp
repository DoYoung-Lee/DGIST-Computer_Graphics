/* -------------------------------- 
	References:

   -------------------------------- */

// #include "3DObjectFunc.h"
#include "UserInputFunc.h"
#include "FileFunc.h"

#include <fstream>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>


#define PI 3.141592f

GLuint shader_program;

GLuint vertex_array_obj;
GLuint vertexbuffer_obj;
GLuint colorbuffer_obj;

struct Object {
	float x, y;
	float rot;
	float vel_x, vel_y, vel_rot;
	int vertex_base_index;
	int n_vertices;
};

static void InitializeGlutCallbacks(void) {

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CrossRoad3D");
	InitializeGlutCallbacks();
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
		return 1;
	}
	InitShaders(shader_program, "vertex_shader.glsl", "frag_shader.glsl");

	return 0;
}