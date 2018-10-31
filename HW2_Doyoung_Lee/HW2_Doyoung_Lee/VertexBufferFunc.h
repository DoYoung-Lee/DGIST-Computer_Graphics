#ifndef VERTEX_BUFFER_FUNC_H
#define VERTEX_BUFFER_FUNC_H

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

GLuint shader_program;

GLuint vertex_array_obj;
GLuint vertexbuffer_obj;
GLuint colorbuffer_obj;
GLuint indexbufer_obj;

std::vector<glm::vec3> vertices_obj;
std::vector<glm::vec3> colors_obj;
std::vector<unsigned int> indices_obj;

void DoInitShader();
void InitObjectsVertex();
static void CreateVertexBuffer();
static void RenderScene2DCB();
static void RenderScene3DCB();

#endif