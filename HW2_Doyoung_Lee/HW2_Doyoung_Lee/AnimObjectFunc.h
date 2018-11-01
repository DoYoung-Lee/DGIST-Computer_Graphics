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
	int n_vertices;
	int vertex_base_index;
public:
	Object(ObjectList*);
	float GetPosition();
	void SetPosition(float, float, float);
	bool Collide(Object*);
	void UpdatePosition();
	void StepSelf();
	void DrawSelf();
};

void InitObject();

#endif