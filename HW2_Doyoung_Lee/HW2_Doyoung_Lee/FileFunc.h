#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

std::string ReadShader(const char*);
void InitShaders(GLuint*, const char*, const char*);

struct int3 {
	int3(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
	int v, vt, vn;
};

bool ObjLoader(const char*, std::vector<glm::vec3>*, std::vector<glm::vec3>*, std::vector<unsigned int>*, int);

#endif