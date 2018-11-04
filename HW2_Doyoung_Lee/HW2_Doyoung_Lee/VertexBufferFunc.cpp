/* --------------------------------
	References:
	"http://www.opengl-tutorial.org/kr/intermediate-tutorials/tutorial-9-vbo-indexing/" to implement indexed VBO
   -------------------------------- */



#include "VertexBufferFunc.h"
#include "FileFunc.h"

#include <glm/gtc/matrix_transform.hpp>

void DoInitShader(GLuint *shader_program) {
	InitShaders(shader_program, "vertex_shader_3d.glsl", "frag_shader_3d.glsl");
}

void InitObjectsVertex() {
	vertices_obj = {
		{ -0.25f, 0.0f, -0.25f }, // Player begin
		{ 0.25f, 0.0f, -0.25f },
		{ 0.25f, 0.5f, -0.25f },
		{ -0.25f, 0.5f, -0.25f },
		{ -0.25f, 0.0f, 0.25f },
		{ 0.25f, 0.0f, 0.25f },
		{ 0.25f, 0.5f, 0.25f },
		{ -0.25f, 0.5f, 0.25f }, // Player end
		{ -0.5f, 0.0f, -0.5f }, // Temp obj begin
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 1.0f, -0.5f },
		{ -0.5f, 1.0f, -0.5f },
		{ -0.5f, 0.0f, 0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ 0.5f, 1.0f, 0.5f },
		{ -0.5f, 1.0f, 0.5f }, // Temp obj end
		{ -0.5f, 0.0f, -0.5f }, // Start tile begin
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ -0.5f, 0.0f, 0.5f }, // Start tile end
		{ -0.5f, 0.0f, -0.5f }, // Grass begin
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ -0.5f, 0.0f, 0.5f }, // Grass end
		{ -0.5f, 0.0f, -0.5f }, // Road tile begin
		{ -0.1f, 0.0f, -0.5f },
		{ -0.1f, 0.0f, 0.5f },
		{ -0.5f, 0.0f, 0.5f },
		{ -0.1f, 0.0f, -0.2f },
		{ 0.1f, 0.0f, -0.2f },
		{ 0.1f, 0.0f, 0.2f },
		{ -0.1f, 0.0f, 0.2f },
		{ 0.1f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f }, 
		{ 0.1f, 0.0f, 0.5f },
		{ -0.1f, 0.0f, -0.5f },
		{ 0.1f, 0.0f, -0.5f },
		{ 0.1f, 0.0f, -0.2f },
		{ -0.1f, 0.0f, -0.2f },
		{ -0.1f, 0.0f, 0.2f },
		{ 0.1f, 0.0f, 0.2f },
		{ 0.1f, 0.0f, 0.5f },
		{ -0.1f, 0.0f, 0.5f }, // Road tile end
	};
	
	colors_obj = {
		{ 1.0f, 0.0f, 0.0f }, // Player begin
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }, // Player end
		{ 0.0f, 1.0f, 1.0f }, // Temp obj begin
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f }, // Temp obj end
		{ 0.59f, 0.29f, 0.0f }, // Start tile begin
		{ 0.59f, 0.29f, 0.0f },
		{ 0.59f, 0.29f, 0.0f },
		{ 0.59f, 0.29f, 0.0f }, // Start tile end
		{ 0.0f, 1.0f, 0.0f }, // Grass tile begin
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }, // Grass tile end
		{ 0.1f, 0.1f, 0.1f }, // Road tile begin
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		{ 0.8f, 0.8f, 0.0f }, // Road tile end

	}; 
	
	indices_obj = {
		0, 1, 3, // Player begin
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
		7, 6, 3, // Player end (0 ~ 11)
		8, 9, 11, // Temp obj begin
		8, 11, 12,
		8, 12, 9,
		10, 9, 11,
		10, 11, 14,
		10, 14, 9,
		13, 9, 12,
		13, 12, 14,
		13, 14, 9,
		15, 11, 12,
		15, 12, 14,
		15, 14, 11, // Temp obj end (12 ~ 23)
		16, 17, 18,	18, 19, 16, // Start tile (24 ~ 25)
		20, 21, 22, 22, 23, 20, // Grass tile (26 ~ 27)
		24, 25, 26, // Road tile begin
		26, 27, 24, 
		28, 29, 30,
		30, 31, 28,
		32, 33, 34,
		34, 35, 32,
		36, 37, 38,
		38, 39, 36,
		40, 41, 42,
		42, 43, 40, //Road tile end (28 ~ 37)
	};
	
	ObjLoader("object\\policecar\\police_car.obj", &vertices_obj, &colors_obj, &indices_obj, vertices_obj.size());
	ObjLoader("object\\taxi\\taxi.obj", &vertices_obj, &colors_obj, &indices_obj, vertices_obj.size());
	ObjLoader("object\\truck\\truck_blue.obj", &vertices_obj, &colors_obj, &indices_obj, vertices_obj.size());
	ObjLoader("object\\christmastree\\christmas_tree.obj", &vertices_obj, &colors_obj, &indices_obj, vertices_obj.size());
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

	glGenBuffers(1, &indexbuffer_obj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer_obj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_obj.size() * sizeof(unsigned int), &indices_obj[0], GL_STATIC_DRAW);
}