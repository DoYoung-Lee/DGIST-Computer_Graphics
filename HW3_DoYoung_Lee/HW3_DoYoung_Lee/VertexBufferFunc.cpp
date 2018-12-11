/* --------------------------------
	References:
	"http://www.opengl-tutorial.org/kr/intermediate-tutorials/tutorial-9-vbo-indexing/"
	and "https://stackoverflow.com/questions/23458089/multiple-objects-drawing-opengl" to implement indexed VBO
   -------------------------------- */


#include "VertexBufferFunc.h"
#include "FileFunc.h"

#include <glm/gtc/matrix_transform.hpp>
#include <map>

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Declaration				--------	--------

//	--------	--------	--------	--------	--------	--------

GLuint shader_program;

// VAO, VBOs
GLuint vertex_array_obj;
GLuint vertex_buffer_obj;
GLuint texture_coord_buffer_obj;
GLuint normal_buffer_obj;
GLuint index_buffer_obj;

// Contents for VBOs
std::vector<glm::vec3> vertices_obj;
// std::vector<glm::vec3> colors_obj;
std::vector<glm::vec2> texture_coords_obj;
std::vector<glm::vec3> normals_obj;
std::vector<unsigned int> indices_obj;

// VBO map
std::map<std::string, Model> object_model_map;

void DoInitShader() {
	InitShaders(&shader_program, "vertex_shader_ha.glsl", "frag_shader_ha.glsl");
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	VBOs Initialization		--------	--------

//	--------	--------	--------	--------	--------	--------

void InitObjectsVertex() {
	int temp = 0;

	vertices_obj = {
		{ -0.5f, 0.0f, -0.5f }, // Cube begin (0)
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 1.0f, -0.5f },
		{ -0.5f, 1.0f, -0.5f },
		{ -0.5f, 0.0f, 0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ 0.5f, 1.0f, 0.5f },
		{ -0.5f, 1.0f, 0.5f }, // Cube end (7)
		{ -0.5f, 0.0f, -0.5f }, // Plane tile begin (8)
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ -0.5f, 0.0f, 0.5f }, // Plane tile end (11)
		{ -0.5f, 0.0f, -0.5f }, // Road tile begin (12)
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
		{ -0.1f, 0.0f, 0.5f }, // Road tile end (31)
		{ -0.2f, 0.0f, -0.2f }, // Player torso begin (32)
		{ 0.2f, 0.0f, -0.2f },
		{ 0.2f, 0.5f, -0.2f },
		{ -0.2f, 0.5f, -0.2f },
		{ -0.2f, 0.0f, 0.2f },
		{ 0.2f, 0.0f, 0.2f },
		{ 0.2f, 0.5f, 0.2f },
		{ -0.2f, 0.5f, 0.2f }, // Player torso end (39)
		{ -0.1f, 0.0f, -0.05f }, // Player arm begin (40)
		{ 0.1f, 0.0f, -0.05f },
		{ 0.1f, 0.2f, -0.05f },
		{ -0.1f, 0.2f, -0.05f },
		{ -0.1f, 0.0f, 0.05f },
		{ 0.1f, 0.0f, 0.05f },
		{ 0.1f, 0.2f, 0.05f },
		{ -0.1f, 0.2f, 0.05f }, // Player arm end (47)
	};
			
	indices_obj = {
		0, 1, 3, // Cube begin
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
		7, 6, 3, // Cube end (0 ~ 11)
		8, 9, 10, // Tile begin
		10, 11, 8, // Tile end (12 ~ 13)
		12, 13, 14, // Road tile begin
		14, 15, 12, 
		16, 17, 18,
		18, 19, 14,
		20, 21, 22,
		22, 23, 20,
		24, 25, 26,
		26, 27, 24,
		28, 29, 30,
		30, 31, 28, //Road tile end (14 ~ 23)
		32, 33, 35, // Player torso begin
		32, 35, 36,
		32, 36, 33,
		34, 33, 35,
		34, 35, 38,
		34, 38, 33,
		37, 33, 36,
		37, 36, 38,
		37, 38, 33,
		39, 35, 36,
		39, 36, 38,
		39, 38, 35, // Player torso end (24 ~ 35)
		40, 41, 43, // Player arm begin
		40, 43, 44,
		40, 44, 41,
		42, 41, 43,
		42, 43, 46,
		42, 46, 41,
		45, 41, 44,
		45, 44, 46,
		45, 46, 41,
		47, 43, 44,
		47, 44, 46,
		47, 46, 43,// Player arm end (36 ~ 47)
	};
	
	temp = vertices_obj.size();
	texture_coords_obj.assign(temp, glm::vec2{0, 0});
	normals_obj.assign(temp, glm::vec3{ 0, 0, 0 });

	// Manually set object model map according to VBOs
	object_model_map["cube"] = Model{ 12, 0, 1.0f };
	object_model_map["tile"] = Model{ 2, 12, 1.0f };
	object_model_map["road"] = Model{ 10, 14, 1.0f };
	object_model_map["torso"] = Model{ 12, 24, 1.0f };
	object_model_map["arm"] = Model{ 12, 36, 1.0f };
	
	Model temp_model;
	temp_model.scale = 1.0f;
	// Import police car model
	temp_model.vertex_base_index = indices_obj.size() / 3;
	temp_model.n_indices = ObjLoader("./object\\policecar\\police_car.obj", &vertices_obj, &texture_coords_obj, &normals_obj, &indices_obj);
	object_model_map["police_car"] = temp_model;
	
	// Import taxi model
	temp_model.vertex_base_index = indices_obj.size() / 3;
	temp_model.n_indices = ObjLoader("./object\\taxi\\taxi.obj", &vertices_obj, &texture_coords_obj, &normals_obj, &indices_obj);
	object_model_map["taxi"] = temp_model;

	// Import truck model
	temp_model.vertex_base_index = indices_obj.size() / 3;
	temp_model.n_indices = ObjLoader("./object\\truck\\truck_blue.obj", &vertices_obj, &texture_coords_obj, &normals_obj, &indices_obj);
	object_model_map["truck"] = temp_model;

	// Import christmas tree model
	temp_model.vertex_base_index = indices_obj.size() / 3;
	temp_model.n_indices = ObjLoader("./object\\christmastree\\christmas_tree.obj", &vertices_obj, &texture_coords_obj, &normals_obj, &indices_obj);
	object_model_map["christmas_tree"] = temp_model;
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Display Callback Functions	----	--------

//	--------	--------	--------	--------	--------	--------

void RenderSceneObjCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// 1st attribute buffer: vertices
	GLint position_loc = glGetAttribLocation(shader_program, "Position");
	glEnableVertexAttribArray(position_loc);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 2nd attribute buffer: texture coordinates
	GLint texture_coord_loc = glGetAttribLocation(shader_program, "Texture_coord");
	glEnableVertexAttribArray(texture_coord_loc);
	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_obj);
	glVertexAttribPointer(texture_coord_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// 3rd attribute buffer: vertex normals
	GLint normal_loc = glGetAttribLocation(shader_program, "Normal");
	glEnableVertexAttribArray(normal_loc);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_obj);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Model view matrix of perspective
	GLint matrix_loc = glGetUniformLocation(shader_program, "MVP");
	glm::mat4 MVP = CreateMVP();
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_obj);
	
	// Draw each objects
	glEnable(GL_DEPTH_TEST);
	room.func[Draw](&room);
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(texture_coord_loc);
	glDisableVertexAttribArray(normal_loc);

	glutSwapBuffers();
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Create VBOs				--------	--------

//	--------	--------	--------	--------	--------	--------

void CreateVertexBuffer() {
	InitObjectsVertex();

	glCreateVertexArrays(1, &vertex_array_obj);
	glBindVertexArray(vertex_array_obj);

	// Vertex positions
	glGenBuffers(1, &vertex_buffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW); // float array case
	glBufferData(GL_ARRAY_BUFFER, vertices_obj.size() * sizeof(glm::vec3), &vertices_obj[0], GL_STATIC_DRAW); // float vector case

	// Texture coordinates
	glGenBuffers(1, &texture_coord_buffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_obj);
	glBufferData(GL_ARRAY_BUFFER, texture_coords_obj.size() * sizeof(glm::vec2), &texture_coords_obj[0], GL_STATIC_DRAW);

	// Vertex normals
	glGenBuffers(1, &normal_buffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_obj);
	glBufferData(GL_ARRAY_BUFFER, normals_obj.size() * sizeof(glm::vec3), &normals_obj[0], GL_STATIC_DRAW);

	// Indices for IBO
	glGenBuffers(1, &index_buffer_obj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_obj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_obj.size() * sizeof(unsigned int), &indices_obj[0], GL_STATIC_DRAW);
}