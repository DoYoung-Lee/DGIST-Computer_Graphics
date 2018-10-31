#include "VertexBufferFunc.h"
#include "FileFunc.h"

#include <glm/gtc/matrix_transform.hpp>

void DoInitShader(GLuint *shader_program) {
	InitShaders(shader_program, "vertex_shader_3d.glsl", "frag_shader_3d.glsl");
}

void InitObjectsVertex() {
	vertices_obj.push_back({ 0.0f, 0.0f, 0.0f });
	vertices_obj.push_back({ 1.0f, 0.0f, 0.0f });
	vertices_obj.push_back({ 1.0f, 1.0f, 0.0f });
	vertices_obj.push_back({ 0.0f, 1.0f, 0.0f });
	vertices_obj.push_back({ 0.0f, 0.0f, 1.0f });
	vertices_obj.push_back({ 1.0f, 0.0f, 1.0f });
	vertices_obj.push_back({ 1.0f, 1.0f, 1.0f });
	vertices_obj.push_back({ 0.0f, 1.0f, 1.0f });

	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });
	colors_obj.push_back({ 1.0f, 1.0f, 1.0f });

	indices_obj = {
		0, 1, 3, // Cube 1 begin
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
		7, 6, 3, // Cube 1 end
	};
}

static void CreateVertexBuffer() {
	InitObjectsVertex();

	glCreateVertexArrays(1, &vertex_array_obj);
	glBindVertexArray(vertex_array_obj);

	glGenBuffers(1, &vertexbuffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW); // float array case
	glBufferData(GL_ARRAY_BUFFER, vertices_obj.size() * sizeof(glm::vec3), &vertices_obj[0], GL_STATIC_DRAW); // float vector case

	glGenBuffers(1, &colorbuffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj);
	glBufferData(GL_ARRAY_BUFFER, colors_obj.size() * sizeof(glm::vec3), &colors_obj[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexbufer_obj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbufer_obj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_obj.size() * sizeof(unsigned int), &indices_obj[0], GL_STATIC_DRAW);
}