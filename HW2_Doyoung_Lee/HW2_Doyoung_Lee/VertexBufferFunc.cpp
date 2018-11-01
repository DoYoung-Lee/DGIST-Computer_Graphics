#include "VertexBufferFunc.h"
#include "FileFunc.h"

#include <glm/gtc/matrix_transform.hpp>

void DoInitShader(GLuint *shader_program) {
	InitShaders(shader_program, "vertex_shader_3d.glsl", "frag_shader_3d.glsl");
}

void InitObjectsVertex() {
	vertices_obj = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
	};
	
	colors_obj = {
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
	};
	
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

void RenderScene2DCB() { // legacy code for 2d drawing
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

void RenderScene3DCB() { // legacy code for simple 3d rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make model view matrix
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

	// Scale object
	GLint scale_loc = glGetUniformLocation(shader_program, "Scale");
	glUniform1f(scale_loc, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);

	glutSwapBuffers();
}

void CreateVertexBuffer() {
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