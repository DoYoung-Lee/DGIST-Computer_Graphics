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
		(*iter)->DrawSelf(*iter);
		iter++;
	}
}

// ----------- ----------- ----------- ----------- -----------

Object::Object(glm::vec3 init_pos, int init_n_indices, int init_vbi) {
	position = init_pos;
	velocity = { 0, 0, 0 };
	n_indices = init_n_indices;
	vertex_base_index = init_vbi;
	scale = 1.0f;
}

void Object::SetModel(int model_indices, int model_base_index, float model_scale = 1.0f) {
	n_indices = model_indices;
	vertex_base_index = model_base_index;
	scale = model_scale;
}

glm::vec3 Object::GetPosition() {
	return position;
}

glm::vec3 Object::GetVelocity() {
	return velocity;
}

void Object::SetPosition(glm::vec3 new_position) {
	position = new_position;
}

void Object::SetVelocity(glm::vec3 new_velocity) {
	velocity = new_velocity;
}

/*
bool Object::Collide(Object* target_obj) {
	bool is_x_collide = abs(target_obj->position.x - position.x) < (target_obj->collision_mask.x / 2 + collision_mask.x / 2); // target_obj->position.x;
	bool is_y_collide = abs(target_obj->position.y - position.y) < (target_obj->collision_mask.y / 2 + collision_mask.y / 2);
	bool is_z_collide = abs(target_obj->position.z - position.z) < (target_obj->collision_mask.z / 2 + collision_mask.z / 2);

	return (is_x_collide & is_y_collide & is_z_collide);
}
*/
void Object::StepSelf() {
	func[0](this);
}

void Object::DrawSelf(Object* self) {
	GLint x_loc = glGetUniformLocation(shader_program, "x");
	GLint y_loc = glGetUniformLocation(shader_program, "y");
	GLint z_loc = glGetUniformLocation(shader_program, "z");
	GLint scale_loc = glGetUniformLocation(shader_program, "Scale");
	
	glUniform1f(x_loc, position.x);
	glUniform1f(y_loc, position.y);
	glUniform1f(z_loc, position.z);
	glUniform1f(scale_loc, scale);

	glDrawElements(GL_TRIANGLES, 3 * n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (vertex_base_index * sizeof(glm::vec3)));
}

// ----------- ----------- ----------- ----------- -----------

Object* InitObject(ObjectList* target_obj_list) {
	// create player marker
	Object* player = new Object({ 0, 0, -0.5 }, 158, 38);
	player->collision_mask = { 1.0f, 1.0f, 1.0f };
	(*target_obj_list).CreateObject(player);

	player->func[0] = [](Object* self) {
		glm::vec3 current_position = self->GetPosition();
		glm::vec3 current_vel = self->GetVelocity();
		glm::vec3 new_position = current_position + current_vel;
		self->SetPosition(new_position);
	};

	std::srand(std::time(nullptr));
	
	Object* temp_obj = new Object({ 2.0f, 0.0f, 2.0f }, 158, 38);
	(*target_obj_list).CreateObject(temp_obj);
	temp_obj->SetModel(158, 38, true);

	temp_obj->func[0] = [](Object* self) {
		glm::vec3 current_position = self->GetPosition();
		glm::vec3 current_vel = self->GetVelocity();
		glm::vec3 new_position = current_position + current_vel;
		self->SetPosition(new_position);
	};

	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < room_col; j++) {
			Object* start_tile = new Object({ i, 0, j - room_col / 2.0 }, 2, 24);
			(*target_obj_list).CreateObject(start_tile);
		}
	}
	
	// fill grass and road
	for (int i = 4; i < room_row; i++) {
		float random = (std::rand() / 1.0) / RAND_MAX;
		int row_tile; // 26 = Grass tile, 28 = Road tile
		int temp_n_indices;
		if (random > 0.5) {
			row_tile = 28;
			temp_n_indices = 10;
			random = (std::rand() / 1.0) / RAND_MAX;
		}
		else {
			row_tile = 26;
			temp_n_indices = 2;
			random += 0.4;
		}
		for (int j = 0; j < room_col; j++) {
			
			Object* road_grass = new Object({ i, 0, j - room_col / 2.0 }, temp_n_indices, row_tile);
			(*target_obj_list).CreateObject(road_grass);
		}
	}
	// create tree on the road
		 
	// create car on the road

	// build wall

	*/
	return player;
}
