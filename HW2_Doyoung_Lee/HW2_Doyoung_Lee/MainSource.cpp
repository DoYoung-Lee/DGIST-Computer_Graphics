/* -------------------------------- 
	References:
	main reference is lecture note provided by Sunghyun Cho, ICE, DGIST.
   -------------------------------- */

#include "UserInputFunc.h"
#include "FileFunc.h"
#include "TimerFunc.h"
// #include "VertexFunc.h"

#include <fstream>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592f

GLuint shader_program;

GLuint vertex_array_obj;
GLuint vertexbuffer_obj;
GLuint colorbuffer_obj;
GLuint indexbufer_obj;

struct Object {
	float x, y;
	float rot;
	float vel_x, vel_y, vel_rot;
	int vertex_base_index;
	int n_vertices;
};

static void CreateVertexBuffer() {
	glCreateVertexArrays(1, &vertex_array_obj);
	glBindVertexArray(vertex_array_obj);

	float vertices[]{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
	};	
	
	glGenBuffers(1, &vertexbuffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};
	glGenBuffers(1, &colorbuffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0, 1, 3,
		0, 3, 4,
		0, 4, 1,
		2, 1, 3,
		2, 3, 6,
		2, 6, 1,
		5, 1, 4,
		5, 4, 6,
		5, 6, 1,
		7, 3, 4,
		7, 4, 6,
		7, 6, 3,
	};

	glGenBuffers(1, &indexbufer_obj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbufer_obj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void InitAnimObjects() {

}

static void RenderScene2DCB() {
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
	
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
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
	InitShaders(shader_program, "vertex_shader_3d.glsl", "frag_shader_3d.glsl");
	CreateVertexBuffer(); // Create vertex buffer using vertice data
	InitAnimObjects(); // Initialize animation objects
	glutMainLoop();

	return 0;
}