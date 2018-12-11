/* --------------------------------
	References :
	http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/
	https://stackoverflow.com/questions/14887012/object-loader-in-opengl
	https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp#L100

   -------------------------------- */

#include "FileFunc.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string ReadShader(const char* pFileName) {
	std::string outFile;

	std::ifstream f(pFileName);

	if (!f.is_open())
		return "";

	std::string line;
	while (std::getline(f, line)) {
		outFile.append(line);
		outFile.append("\n");
	}
	return outFile;
}

void InitShaders(GLuint *shader_program, const char* path_vertex_shader, const char* path_fragment_shader) {
	GLint success;
	GLchar error_log[1024];

	*shader_program = glCreateProgram();
	if (shader_program == 0) {
		std::cerr << "Error creating shader program" << std::endl;
		exit(1);
	}

	std::string vertex_shader_text = ReadShader(path_vertex_shader);
	std::string frag_shader_text = ReadShader(path_fragment_shader);
	
	GLuint vertex_shader_obj = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* p[1];
	GLint lengths[1];
	p[0] = vertex_shader_text.c_str();
	lengths[0] = vertex_shader_text.size();
	glShaderSource(vertex_shader_obj, 1, p, lengths);
	glCompileShader(vertex_shader_obj);
	glGetShaderiv(vertex_shader_obj, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader_obj, sizeof(error_log), NULL, error_log);
		std::cerr << "Error compiling Vertex Shader: " << error_log << std::endl;
		exit(1);
	}

	GLuint frag_shader_obj = glCreateShader(GL_FRAGMENT_SHADER);
	p[0] = frag_shader_text.c_str();	lengths[0] = frag_shader_text.size();
	glShaderSource(frag_shader_obj, 1, p, lengths);
	glCompileShader(frag_shader_obj);
	glGetShaderiv(frag_shader_obj, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag_shader_obj, 1024, NULL, error_log);
		std::cerr << "Error compiling Fragment Shader: " << error_log << std::endl;
		exit(1);
	}

	glAttachShader(*shader_program, vertex_shader_obj);
	glAttachShader(*shader_program, frag_shader_obj);

	glLinkProgram(*shader_program);
	glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*shader_program, sizeof(error_log), NULL, error_log);
		std::cerr << "Error linking shader program: " << error_log << std::endl;
		exit(1);
	}

	glValidateProgram(*shader_program);
	glGetProgramiv(*shader_program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*shader_program, sizeof(error_log), NULL, error_log);
		std::cerr << "Invalid shader program: " << error_log << std::endl;
		exit(1);
	}

	glUseProgram(*shader_program);
}

int ObjLoader(const char* path, std::vector<glm::vec3> *out_vertices, std::vector<glm::vec2> *out_uvs, std::vector<glm::vec3> *out_normals, std::vector<unsigned int> *out_indices) {
	std::ifstream obj_stream(path);
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	std::string line_string;
	int base_index = (*out_vertices).size();
	int out_n_indices = 0;

	while (std::getline(obj_stream, line_string)) {
		std::istringstream line_stream(line_string);
		std::string line_type;
		line_stream >> line_type;
		// Vertex case
		if (line_type == "v") {
			float x = 0.0, y = 0.0, z = 0.0;
			line_stream >> x >> y >> z;
			temp_vertices.push_back(glm::vec3(x, y, z));
		}
		// Texture coordinate case
		else if (line_type == "vt") {
			float u = 0.0, v = 0.0;
			line_stream >> u >> v;
			temp_uvs.push_back(glm::vec2(u, v));
		}
		// Normal vector case
		else if (line_type == "vn") {
			float i = 0.0, j = 0.0, k = 0.0;
			line_stream >> i >> j >> k;
			temp_normals.push_back(glm::vec3(i, j, k));
		}
		// Face case
		else if (line_type == "f") {
			std::string ref_string;
			for (int i = 0; i < 3; i++) {
				line_stream >> ref_string;
				
				std::istringstream ref_stream(ref_string);
				std::string v_string, vt_string, vn_string;

				std::getline(ref_stream, v_string, '/');
				std::getline(ref_stream, vn_string, '/');
				std::getline(ref_stream, vt_string, '/');

				vertex_indices.push_back(atoi(v_string.c_str()));
				uv_indices.push_back(atoi(vt_string.c_str()));
				normal_indices.push_back(atoi(vn_string.c_str()));
			}
		}
	}

	// Append to VBOs according to the indices
	out_n_indices = vertex_indices.size();
	for (unsigned int i = 0; i < out_n_indices; i++) {
		glm::vec3 vertex = temp_vertices[vertex_indices[i] - 1];
		glm::vec2 uv = temp_uvs[uv_indices[i] - 1];
		glm::vec3 normal = temp_normals[normal_indices[i] - 1];
		(*out_vertices).push_back(vertex);
		(*out_uvs).push_back(uv);
		(*out_normals).push_back(normal);
		(*out_indices).push_back(i + base_index);
	}
	
	obj_stream.close();
	return out_n_indices / 3;
}