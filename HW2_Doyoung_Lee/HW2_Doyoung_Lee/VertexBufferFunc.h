#ifndef VERTEX_BUFFER_FUNC_H
#define VERTEX_BUFFER_FUNC_H

#include "AnimObjectFunc.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

extern GLuint shader_program;
extern GLuint vertex_array_obj;
extern GLuint vertexbuffer_obj;
extern GLuint colorbuffer_obj;
extern GLuint indexbuffer_obj;

extern std::vector<glm::vec3> vertices_obj;
extern std::vector<glm::vec3> colors_obj;
extern std::vector<unsigned int> indices_obj;

extern ObjectList room;

void DoInitShader(GLuint*);
void RenderScene2DCB();
void RenderScene3DCB();
void RenderSceneObjCB();
void InitObjectsVertex();
void CreateVertexBuffer();

#endif