/* -------------------------------- 
	References:

   -------------------------------- */

// #include "3DObjectFunc.h"
#include "UserInputFunc.h"
// #include "ShaderFunc.h"

#include <fstream>
#include <string>

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

static void InitShaders() {
	GLint success;
	GLchar error_log[1024];


}

static void InitializeGlutCallbacks(void) {

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CrossRoad3D");
	InitializeGlutCallbacks();


	return 0;
}