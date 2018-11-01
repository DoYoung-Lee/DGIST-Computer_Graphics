#ifndef ANIM_OBJ_FUNC_H
#define ANIM_OBJ_FUNC_H

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

#include <vector>

extern GLuint shader_program;

class Object;

class ObjectList {
private:
	std::vector<Object*> list; // Store address of each object
	int n_objects;
	int vertex_list_index;
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
	int n_indices;
	int vertex_base_index;
public:
	Object();
	void SetModel(int, int);
	float GetPosition();
	void SetPosition(glm::vec3);
	void SetVelocity(glm::vec3);
	bool Collide(Object*);
	void UpdatePosition();
	void StepSelf();
	void DrawSelf();
};

Object* InitObject(ObjectList*);

#endif