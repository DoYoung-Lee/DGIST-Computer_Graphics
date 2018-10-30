/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 to make Object class.
   -------------------------------- */

#include <GL/glew.h>
#include <gl/freeglut.h>

#include <vector>

// ----------- ----------- ----------- ----------- -----------

class ObjectList {
private:
	std::vector<Object*> list; // Store address of the first element of each object
	int n_objects;
	int vertex_list_index;
public:
	ObjectList::ObjectList();
	void CreateObject();
	void DestroyObject();
}

ObjectList::ObjectList() {
	n_objects = 0;
	vertex_list_index = 0;
}

void ObjectList::CreateObject(Object &new_obj) {
	list.push_back(&new_obj);
	n_objects += 1;
}

void ObjectList::DestroyObject(Object &target_obj) {
	list.erase(&target_obj);
	n_objects -= 1;
}

// ----------- ----------- ----------- ----------- -----------

class Object {
private:
	vec3 position;
	vec3 velocity;
	int n_vertices;
	int vertex_base_index;
public:
	Object::Object();
	float GetPosition();
	void SetPosition(float, float, float);
	bool Collide(&Object);
	void UpdatePosition();
};

Object::Object(ObjectList &obj_list) {
	obj_list.CreateObject(this);
}

float AnimObject::GetPosition() {
	return position.x, position.y, position.z;
}

void AnimObject::SetPosition(int new_x, int new_y, int new_z) {
	position.x = new_x;
	position.y = new_y;
	position.z = new_z;
}

bool AnimObject::Collide(Object* target_obj) {
	bool is_x_collide = target_obj->position.x;
	bool is_y_collide = ;
	bool is_z_collide = ;

	return (is_x_collide | is_y_collide | is_z_collide);
}

void AnimObject::UpdatePosition() {
	float new_x = position.x + velocity.x;
	float new_y = position.y + velocity.y;
	float new_z = position.z + velocity.z;

	// Loop for check
}