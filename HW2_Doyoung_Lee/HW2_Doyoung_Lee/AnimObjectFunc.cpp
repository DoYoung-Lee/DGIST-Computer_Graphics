/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 to make Object class.
   -------------------------------- */

#include "AnimObjectFunc.h"
#include <algorithm>
#include <ctime>
#include <stdlib.h>

// ----------- ----------- ----------- ----------- -----------

ObjectList::ObjectList() {
	
}

void ObjectList::CreateObject(Object *new_obj) {
	list.push_back(new_obj);
}

void ObjectList::DestroyObject(Object *target_obj) {
	list.erase(std::find(list.begin(), list.end(), target_obj));
	delete target_obj;
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
	position = { 0, 0, 0 };
	velocity = { 0, 0, 0 };
	collision_mask = { 0, 0, 0 };
	n_indices = 0;
	vertex_base_index = 0;
}

Object::Object(glm::vec3 init_pos, int init_n_indices, int init_vbi) {
	position = init_pos;
	velocity = { 0, 0, 0 };
	collision_mask = { 0, 0, 0 };
	n_indices = init_n_indices;
	vertex_base_index = init_vbi;
}

void Object::SetModel(int model_indices, int model_base_index) {
	n_indices = model_indices;
	vertex_base_index = model_base_index;
}

glm::vec3 Object::GetPosition() {
	return position;
}

void Object::SetPosition(glm::vec3 new_position) {
	position = new_position;
}

void Object::SetVelocity(glm::vec3 new_velocity) {
	velocity = new_velocity;
}

void Object::SetCollisionMask(glm::vec3 new_collision_mask) {
	collision_mask = new_collision_mask;
}

bool Object::Collide(Object* target_obj) {
	bool is_x_collide = false; // target_obj->position.x;
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
//	GLint scale_loc = glGetUniformLocation(shader_program, "Scale");

	glUniform1f(x_loc, position.x);
	glUniform1f(y_loc, position.y);
	glUniform1f(z_loc, position.z);
//	glUniform1f(scale_loc, 1.0f);

	//glDrawElementsBaseVertex(GL_TRIANGLES, 3 * n_indices, GL_UNSIGNED_INT, 0, vertex_base_index);
	glDrawElements(GL_TRIANGLES, 3 * n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (vertex_base_index * sizeof(glm::vec3)));
}

// ----------- ----------- ----------- ----------- -----------

Object* InitObject(ObjectList* target_obj_list) {
	std::srand(std::time(nullptr));
	Object* temp_obj = new Object({ 2.0f, 0.0f, 2.0f }, 12, 12);
	(*target_obj_list).CreateObject(temp_obj);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < room_col; j++) {
			Object* start_tile = new Object({ i, 0, j }, 2, 24);
			(*target_obj_list).CreateObject(start_tile);
		}
	}

	// fill grass and road
	for (int i = 4; i < room_row; i++) {
		float random = (std::rand() / 1.0) / RAND_MAX;
		int row_tile; // 26 = Grass tile, 28 = Road tile
		if (random > 0.8) {
			row_tile = 28;
			random = (std::rand() / 1.0) / RAND_MAX;
		}
		else {
			row_tile = 26;
			random += 0.4;
		}
		for (int j = 0; j < room_col; j++) {
			
			Object* temp_grass = new Object({ i, 0, j }, 2, row_tile);
			(*target_obj_list).CreateObject(temp_grass);
		}
	}
	// create tree on the road
		 
	// create car on the road

	// build wall

	// create player marker
	Object* player = new Object({ 0, 0, 0 }, 12, 0);
	player->SetCollisionMask({ 1.0f, 1.0f, 1.0f });
	(*target_obj_list).CreateObject(player);
	
	return player;
}