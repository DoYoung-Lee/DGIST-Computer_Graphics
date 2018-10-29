#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

std::string ReadShader(const char* pFileName);
void InitShaders(GLuint &shader_program, const char* path_vertex_shader, const char* path_fragment_shader);

bool ObjLoader(
	const char* path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals
);