#ifndef ANIM_OBJ_FUNC_H
#define ANIM_OBJ_FUNC_H

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

#include <vector>

extern GLuint shader_program;
extern int room_row;
extern int room_col;

class Object;

class ObjectList {
private:
	std::vector<Object*> list; // Store address of each object
	//int n_objects;
	//int vertex_list_index;
public:
	ObjectList();
	void CreateObject(Object*);
	void DestroyObject(Object*);
	void StepObjects();
	void DrawObjects();
};

static struct Model
{
	int n_indices;
	int vertex_base_index;
	float scale;
};

class Object {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotation;
	Model model;
public:
	Object(glm::vec3, int, int);
	void SetModel(int, int, float);
	Model GetModel();
	void SetPosition(glm::vec3);
	glm::vec3 GetPosition();
	void SetVelocity(glm::vec3);
	glm::vec3 GetVelocity();
	void SetRotation(glm::vec3);
	glm::vec3 GetRotation();
	void StepSelf();
	void DrawSelf(Object*);
	glm::vec3 collision_mask;

	void(*func[3]) (Object* self);
};

extern Object* player_obj;
extern glm::vec3 player_destination;
extern int move_axis[2];
Object* MakeCarObject(ObjectList*, glm::vec3, int);

Object* InitObject(ObjectList*);

#endif