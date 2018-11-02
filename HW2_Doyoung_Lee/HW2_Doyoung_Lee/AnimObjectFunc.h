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

class Object {
private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 collision_mask;
	int scale;
	int n_indices;
	int vertex_base_index;
	bool wireframe;
public:
	Object();
	Object(glm::vec3, int, int);
	void SetModel(int, int, bool, float);
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3);
	void SetVelocity(glm::vec3);
	void SetCollisionMask(glm::vec3);
	bool Collide(Object*);
	void UpdatePosition();
	void StepSelf();
	void DrawSelf();
};

Object* InitObject(ObjectList*);

#endif