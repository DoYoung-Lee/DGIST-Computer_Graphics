/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 to make Object class.
   -------------------------------- */

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

#include <vector>
#include <algorithm>

// ----------- ----------- ----------- ----------- -----------
class Object;

class ObjectList {
private:
	std::vector<Object*> list; // Store address of the first element of each object
	int n_objects;
	int vertex_list_index;
public:
	ObjectList();
	void CreateObject(Object*);
	void DestroyObject(Object*);
};

ObjectList::ObjectList() {
	n_objects = 0;
	vertex_list_index = 0;
}

void ObjectList::CreateObject(Object *new_obj) {
	list.push_back(new_obj);
	n_objects += 1;
}

void ObjectList::DestroyObject(Object *target_obj) {
	list.erase(std::find(list.begin(), list.end(), target_obj));
	n_objects -= 1;
}

// ----------- ----------- ----------- ----------- -----------

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
};

Object::Object(ObjectList *obj_list) {
	(*obj_list).CreateObject(this);
}

float Object::GetPosition() {
	return position.x, position.y, position.z;
}

void Object::SetPosition(float new_x, float new_y, float new_z) {
	position.x = new_x;
	position.y = new_y;
	position.z = new_z;
}

bool Object::Collide(Object* target_obj) {
	bool is_x_collide = false; //target_obj->position.x;
	bool is_y_collide = false;
	bool is_z_collide = false;

	return (is_x_collide | is_y_collide | is_z_collide);
}

void Object::UpdatePosition() {
	float new_x = position.x + velocity.x;
	float new_y = position.y + velocity.y;
	float new_z = position.z + velocity.z;

	// Loop for check
}