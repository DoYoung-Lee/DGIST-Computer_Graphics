#ifndef VERTEX_BUFFER_FUNC_H
#define VERTEX_BUFFER_FUNC_H

#include "AnimObjectFunc.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

GLuint shader_program;
GLuint vertex_array_obj;
GLuint vertexbuffer_obj;
GLuint colorbuffer_obj;
GLuint indexbuffer_obj;

std::vector<glm::vec3> vertices_obj;
std::vector<glm::vec3> colors_obj;
std::vector<unsigned int> indices_obj;

extern ObjectList room;

void DoInitShader();
void RenderScene2DCB();
void RenderScene3DCB();
void RenderSceneObjCB();
void InitObjectsVertex();
void CreateVertexBuffer();

#endif