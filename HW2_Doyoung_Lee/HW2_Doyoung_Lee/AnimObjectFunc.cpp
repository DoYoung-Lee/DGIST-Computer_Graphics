/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 to make Object class.
   -------------------------------- */

#include "AnimObjectFunc.h"
#include <algorithm>

// ----------- ----------- ----------- ----------- -----------

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

void ObjectList::StepObjects() {
	std::vector<Object*>::iterator iter = list.begin();
	while (iter != list.end()) {
		(*iter)->StepSelf();
		iter++;
	}
}

void ObjectList::DrawObjects() {
	std::vector<Object*>::iterator iter = list.begin();
	while (iter != list.end()) {
		(*iter)->DrawSelf();
		iter++;
	}
}

// ----------- ----------- ----------- ----------- -----------

Object::Object() {
	
}

void Object::SetModel(int model_indices, int model_base_index) {
	n_indices = model_indices;
	vertex_base_index = model_base_index;
}

float Object::GetPosition() {
	return position.x, position.y, position.z;
}

void Object::SetPosition(glm::vec3 new_position) {
	position = new_position;
}

void Object::SetVelocity(glm::vec3 new_velocity) {
	velocity = new_velocity;
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

	// Loop for check collision

	// Update position
	position = { new_x, new_y, new_z };
}

void Object::StepSelf() {
	UpdatePosition();
}

void Object::DrawSelf() {
	GLint x_loc = glGetUniformLocation(shader_program, "x");
	GLint y_loc = glGetUniformLocation(shader_program, "y");
	GLint z_loc = glGetUniformLocation(shader_program, "z");

	glUniform1f(x_loc, position.x);
	glUniform1f(y_loc, position.y);
	glUniform1f(z_loc, position.z);

	glDrawElements(GL_TRIANGLES, 3 * n_indices, GL_UNSIGNED_INT, 0);
}

// ----------- ----------- ----------- ----------- -----------