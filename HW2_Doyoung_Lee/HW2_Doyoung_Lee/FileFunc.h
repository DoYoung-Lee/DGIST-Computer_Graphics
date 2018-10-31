#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

std::string ReadShader(const char*);
void InitShaders(GLuint*, const char*, const char*);

// bool ObjLoader(const char* path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);

#endif