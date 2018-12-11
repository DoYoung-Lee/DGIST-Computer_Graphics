#ifndef VERTEX_BUFFER_FUNC_H
#define VERTEX_BUFFER_FUNC_H

#include "AnimObjectFunc.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

extern Object room;
extern glm::mat4 CreateMVP();
extern enum func_type;

void DoInitShader();
void RenderSceneObjCB();
void InitObjectsVertex();
void CreateVertexBuffer();

#endif