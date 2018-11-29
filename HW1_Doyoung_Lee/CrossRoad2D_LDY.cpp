#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define PI 3.141592f

using namespace std;

GLuint shader_program;

GLuint vertex_array_obj_anim;
GLuint vertexbuffer_obj_anim;
GLuint colorbuffer_obj_anim;
GLuint vertex_array_obj_bg;
GLuint vertexbuffer_obj_bg;
GLuint colorbuffer_obj_bg;

struct Object
{
	float x, y;
	float rot;
	float vel_x, vel_y, vel_rot;
	int vertex_base_index;
	int n_vertices;
};

const int n_animation_objects = 8;
Object animation_objects[n_animation_objects];

string ReadFile(const char* pFileName)
{
	string outFile;

	ifstream f(pFileName);

	if (!f.is_open()) return "";

	string line;
	while (getline(f, line)) {
		outFile.append(line);
		outFile.append("\n");
	}
	return outFile;
}

void TriByQuad(int index) {
	glDrawArrays(GL_TRIANGLES, index, 3);
	glDrawArrays(GL_TRIANGLES, index+1, 3);
}

static void RenderSceneCB()
{
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
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj_bg);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 2nd attribute buffer: colors
	glEnableVertexAttribArray(color_loc);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj_bg);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniform1f(x_loc, -0.9f);
	TriByQuad(0); // Grass
	glUniform1f(x_loc, -0.1f);
	TriByQuad(0);
	glUniform1f(x_loc, 0.1f);
	TriByQuad(0);
	glUniform1f(x_loc, 0.9f);
	TriByQuad(0);
	glUniform1f(x_loc, -0.5f);
	TriByQuad(4); // Road
	for (float i = -1.0f; i < 1.0f; i += 0.3f) {
		glUniform1f(y_loc, i);
		TriByQuad(8);
	}
	glUniform1f(y_loc, 0.0f);	
	glUniform1f(x_loc, 0.5f);
	TriByQuad(4);
	for (float i = -1.0f; i < 1.0f; i += 0.3f) {
		glUniform1f(y_loc, i);
		TriByQuad(8);
	}
	
	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);
	
	//// Draw animation objects
	// 1st attribute buffer: vertices
	glEnableVertexAttribArray(position_loc);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj_anim);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// 2nd attribute buffer: colors
	glEnableVertexAttribArray(color_loc);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj_anim);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw animation objects
	for (int i = 0; i < n_animation_objects; i++) {
		glUniform1f(x_loc, animation_objects[i].x);
		glUniform1f(y_loc, animation_objects[i].y);
		glUniform1f(r_loc, animation_objects[i].rot);
		TriByQuad(animation_objects[i].vertex_base_index);
	}

	glDisableVertexAttribArray(position_loc);
	glDisableVertexAttribArray(color_loc);
		
	glutSwapBuffers();
}

void TimerCallBack(int)
{
	float new_x = animation_objects[0].x + animation_objects[0].vel_x;
	float new_y = animation_objects[0].y + animation_objects[0].vel_y;
	// Tree!
	for (int i = 5; i < n_animation_objects; i++) {
		if (new_x + 0.07f >= animation_objects[i].x - 0.12f && animation_objects[i].x + 0.12f >= new_x - 0.07f) {
			if (new_y + 0.07f >= animation_objects[i].y - 0.12f && animation_objects[i].y + 0.12f >= new_y - 0.07f) {
				new_x = animation_objects[0].x;
				new_y = animation_objects[0].y;
			}
		}
	}

	animation_objects[0].x = new_x;
	animation_objects[0].y = new_y;
	animation_objects[0].rot += animation_objects[0].vel_rot;

	for (int i = 1; i < 3; i++) {
		animation_objects[i].y += animation_objects[i].vel_y;
		animation_objects[i].rot += animation_objects[i].vel_rot;
		if (animation_objects[i].y >= 1.0f + 0.2f) {
			animation_objects[i].y = -1.0f - 0.2f;
			animation_objects[i].vel_y = (rand() % 100 + 30) * 0.0007;
		}
	}
	for (int i = 3; i < 5; i++) {
		animation_objects[i].y += animation_objects[i].vel_y;
		animation_objects[i].rot += animation_objects[i].vel_rot;
		if (animation_objects[i].y <= -1.0f - 0.2f) {
			animation_objects[i].y = 1.0f + 0.2f;
			animation_objects[i].vel_y = -(rand() % 100 + 30) * 0.0007;
		}
	}
	
	// Car accident.
	for (int i = 1; i < 5; i++) {
		if (animation_objects[0].x + 0.07f >= animation_objects[i].x - 0.08f && animation_objects[i].x + 0.08f >= animation_objects[0].x - 0.07f) {
			if (animation_objects[0].y + 0.07f >= animation_objects[i].y - 0.12f && animation_objects[i].y + 0.12f >= animation_objects[0].y - 0.07f) {
				cout << "You lose!" << endl;
				exit(1);
			}
		}
	}

	// Clear condition.
	if (animation_objects[0].x >= 0.93f) {
		cout << "You win!" << endl;
		exit(0);
	}
	
	glutTimerFunc(33, TimerCallBack, 0);
	glutPostRedisplay();
}

void KeyboardCB(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': exit(0);
		break;
	default:
		break;
	}
}

void SpecialCB(int key, int x, int y) {
	switch (key) {
	// Keybaord inputs for player movement
	case GLUT_KEY_UP: animation_objects[0].vel_y = 0.01f;
		break;
	case GLUT_KEY_DOWN: animation_objects[0].vel_y = -0.01f;
		break;
	case GLUT_KEY_LEFT: animation_objects[0].vel_x = -0.01f;
		break;
	case GLUT_KEY_RIGHT: animation_objects[0].vel_x = 0.01f;
		break;
	default:
		break;
	}

}

void SpecialUpCB(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		animation_objects[0].vel_y = 0.0f;
		break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		animation_objects[0].vel_x = 0.0f;
		break;
	default:
		break;
	}
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialCB);
	glutSpecialUpFunc(SpecialUpCB);
	glutTimerFunc(33, TimerCallBack, 0);
}

void CreateVertexBuffer()
{
	// Create vertex array for animation objects.
	glCreateVertexArrays(1, &vertex_array_obj_anim);
	glBindVertexArray(vertex_array_obj_anim);

	float vertices_anim[] = {
		-0.7f, -0.7f, 0.0f, // Player square : begin
		-0.7f, 0.7f, 0.0f,
		0.7f, -0.7f, 0.0f,
		0.7f, 0.7f, 0.0f, // Player square : end
		-0.8f, -1.2f, 0.0f, // Car : begin
		-0.8f, 1.2f, 0.0f,
		0.8f, -1.2f, 0.0f,
		0.8f, 1.2f, 0.0f, // Car : end
		-1.2f, -1.2f, 0.0f, // Tree : begin
		-1.2f, 1.2f, 0.0f,
		1.2f, -1.2f, 0.0f,
		1.2f, 1.2f, 0.0f, // Tree : end
	};
	glGenBuffers(1, &vertexbuffer_obj_anim);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj_anim);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_anim), vertices_anim, GL_STATIC_DRAW);

	float colors_anim[] = {
		1.0f, 0.0f, 1.0f, // Player square : begin
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, // Player square : end
		0.2f, 0.2f, 0.2f, // Car : begin
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f, // Car : end
		0.5f, 0.2f, 0.2f, // Tree : begin
		0.5f, 0.2f, 0.2f,
		0.5f, 0.2f, 0.2f,
		0.5f, 0.2f, 0.2f, // Tree : end
	};
	glGenBuffers(1, &colorbuffer_obj_anim);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj_anim);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors_anim), colors_anim, GL_STATIC_DRAW);

	// Create vertex array for background objects.
	glCreateVertexArrays(1, &vertex_array_obj_bg);
	glBindVertexArray(vertex_array_obj_bg);

	float vertices_bg[] = {
		-2.0f, -11.0f, 0.0f, // Grass : begin
		-2.0f, 11.0f, 0.0f,
		2.0f, -11.0f, 0.0f,
		2.0f, 11.0f, 0.0f, // Grass : end
		-3.0f, -11.0f, 0.0f, // Road : begin
		-3.0f, 11.0f, 0.0f,
		3.0f, -11.0f, 0.0f,
		3.0f, 11.0f, 0.0f, // Road : end
		-0.2f, -1.0f, 0.0f, // Line : begin
		-0.2f, 1.0f, 0.0f,
		0.2f, -1.0f, 0.0f,
		0.2f, 1.0f, 0.0f, // Line : end
	};
	glGenBuffers(1, &vertexbuffer_obj_bg);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obj_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);

	float colors_bg[] = {
		0.2f, 0.6f, 0.2f, // Grass : begin
		0.2f, 0.6f, 0.2f,
		0.2f, 0.6f, 0.2f,
		0.2f, 0.6f, 0.2f, // Grass : end
		0.1f, 0.1f, 0.1f, // Road : begin
		0.1f, 0.1f, 0.1f,
		0.1f, 0.1f, 0.1f,
		0.1f, 0.1f, 0.1f, // Road : end
		1.0f, 1.0f, 1.0f, // Line 1 : begin
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, // Line 1 : end
	};
	glGenBuffers(1, &colorbuffer_obj_bg);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_obj_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors_bg), colors_bg, GL_STATIC_DRAW);

}

void InitializeAnimationObjects()
{
	animation_objects[0].x = -0.9f;
	animation_objects[0].y = 0.0f;
	animation_objects[0].rot = 0.0f;
	animation_objects[0].vel_x = 0.0f;
	animation_objects[0].vel_y = 0.0f;
	animation_objects[0].vel_rot = 0.00f;
	animation_objects[0].vertex_base_index = 0;
	
	animation_objects[1].x = -0.35f;
	animation_objects[1].y = 0.00f;
	animation_objects[1].rot = 0.0f;
	animation_objects[1].vel_x = 0.0f;
	animation_objects[1].vel_y = 0.01f;
	animation_objects[1].vel_rot = 0.0f;
	animation_objects[1].vertex_base_index = 4;
	
	animation_objects[2].x = 0.35f;
	animation_objects[2].y = 0.0f;
	animation_objects[2].rot = 0.0f;
	animation_objects[2].vel_x = 0.0f;
	animation_objects[2].vel_y = 0.04f;
	animation_objects[2].vel_rot = 0.0f;
	animation_objects[2].vertex_base_index = 4;

	animation_objects[3].x = -0.65f;
	animation_objects[3].y = 0.00f;
	animation_objects[3].rot = 0.0f;
	animation_objects[3].vel_x = 0.0f;
	animation_objects[3].vel_y = -0.06f;
	animation_objects[3].vel_rot = 0.0f;
	animation_objects[3].vertex_base_index = 4;

	animation_objects[4].x = 0.65f;
	animation_objects[4].y = 0.00f;
	animation_objects[4].rot = 0.0f;
	animation_objects[4].vel_x = 0.0f;
	animation_objects[4].vel_y = -0.03f;
	animation_objects[4].vel_rot = 0.0f;
	animation_objects[4].vertex_base_index = 4;

	animation_objects[5].x = 0.0f;
	animation_objects[5].y = 0.55f;
	animation_objects[5].rot = 0.0f;
	animation_objects[5].vel_x = 0.0f;
	animation_objects[5].vel_y = 0.00f;
	animation_objects[5].vel_rot = 0.0f;
	animation_objects[5].vertex_base_index = 8;

	animation_objects[6].x = 0.0f;
	animation_objects[6].y = 0.00f;
	animation_objects[6].rot = 0.0f;
	animation_objects[6].vel_x = 0.0f;
	animation_objects[6].vel_y = 0.00f;
	animation_objects[6].vel_rot = 0.0f;
	animation_objects[6].vertex_base_index = 8;

	animation_objects[7].x = 0.0f;
	animation_objects[7].y = -0.55f;
	animation_objects[7].rot = 0.0f;
	animation_objects[7].vel_x = 0.0f;
	animation_objects[7].vel_y = 0.00f;
	animation_objects[7].vel_rot = 0.0f;
	animation_objects[7].vertex_base_index = 8;
}

static void InitializeShaders()
{
	GLint success;
	GLchar error_log[1024];

	shader_program = glCreateProgram();
	if (shader_program == 0) {
		cerr << "Error creating shader program" << endl;
		exit(1);
	}

	string vertex_shader_text = ReadFile("vertex_shader.glsl");
	string frag_shader_text = ReadFile("frag_shader.glsl");

	GLuint vertex_shader_obj = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* p[1];
	GLint lengths[1];
	p[0] = vertex_shader_text.c_str();	lengths[0] = vertex_shader_text.size();
	glShaderSource(vertex_shader_obj, 1, p, lengths);
	glCompileShader(vertex_shader_obj);
	glGetShaderiv(vertex_shader_obj, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader_obj, sizeof(error_log), NULL, error_log);
		cerr << "Error compiling Vertex Shader: " << error_log << endl;
		exit(1);
	}

	GLuint frag_shader_obj = glCreateShader(GL_FRAGMENT_SHADER);
	p[0] = frag_shader_text.c_str();	lengths[0] = frag_shader_text.size();
	glShaderSource(frag_shader_obj, 1, p, lengths);
	glCompileShader(frag_shader_obj);
	glGetShaderiv(frag_shader_obj, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag_shader_obj, 1024, NULL, error_log);
		cerr << "Error compiling Fragment Shader: " << error_log << endl;
		exit(1);
	}

	glAttachShader(shader_program, vertex_shader_obj);
	glAttachShader(shader_program, frag_shader_obj);

	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
		cerr << "Error linking shader program: " << error_log << endl;
		exit(1);
	}

	glValidateProgram(shader_program);
	glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
		cerr << "Invalid shader program: " << error_log << endl;
		exit(1);
	}

	glUseProgram(shader_program);
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CrossRoad");
	InitializeGlutCallbacks();
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		cerr << "Error: " << glewGetErrorString(res) << endl;
		return 1;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	InitializeShaders();
	CreateVertexBuffer();
	InitializeAnimationObjects();
	glutMainLoop();

	return 0;
}
