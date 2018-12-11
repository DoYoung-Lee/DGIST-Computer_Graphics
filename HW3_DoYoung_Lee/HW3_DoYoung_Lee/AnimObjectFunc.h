#ifndef ANIM_OBJ_FUNC_H
#define ANIM_OBJ_FUNC_H

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>

#define PI 3.141592f

extern GLuint shader_program;
extern int room_row;
extern int room_col;

extern glm::vec3 player_destination;
extern int move_axis[3];

enum func_type { Step = 0, Draw = 1, num_func = 2 };

struct Model {
	int n_indices;
	int vertex_base_index;
	float scale;
};

class MatrixStack {
	int max_size;
	std::vector<glm::mat4> matrices_list;
public:
	MatrixStack(int);
	~MatrixStack();
	void PushMatrix(glm::mat4);
	glm::mat4 PopMatrix();
};

class Object {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::vec3 color;
	Model model;
	//glm::mat4 MVP;
	std::vector<Object*> children_list;
public:
	Object(glm::vec3, int, int);
	void SetModel(int, int, float); Model GetModel();
	void SetPosition(glm::vec3); glm::vec3 GetPosition();
	void SetVelocity(glm::vec3); glm::vec3 GetVelocity();
	void SetRotation(glm::vec3); glm::vec3 GetRotation();
	void SetColor(glm::vec3); glm::vec3 GetColor();
	//void SetMVP(glm::mat4); glm::mat4 GetMVP();
	void AppendChild(Object*); std::vector<Object*>* GetChildrenList();

	void(*func[num_func]) (Object* self); // 0 = Step, 1 = Draw
	
	glm::vec3 collision_mask;
};

extern std::map<std::string, Model> object_model_map;

Object* MakeTileObject(Object*, glm::vec3, int);
Object* MakeCarObject(Object*, glm::vec3, int);
void MakeTreeObject(Object*, glm::vec3);
void MakeWallObject(Object*, glm::vec3);
Object* MakePlayerObject(Object*, glm::vec3);

extern Object* player_obj;
Object* LeftArm(Object*);
Object* LeftLowerArm(Object*);
Object* RightArm(Object*);
Object* RightLowerArm(Object*);
Object* InitObject(Object*);

#endif