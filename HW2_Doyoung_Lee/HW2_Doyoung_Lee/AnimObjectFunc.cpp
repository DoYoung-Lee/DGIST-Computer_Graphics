/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 to make Object class.
   -------------------------------- */

#include "AnimObjectFunc.h"
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>

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
	model.n_indices = init_n_indices;
	model.vertex_base_index = init_vbi;
	model.scale = 1.0f;

	func[0] = [](Object* self) {};

	func[1] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		GLint x_loc = glGetUniformLocation(shader_program, "x");
		GLint y_loc = glGetUniformLocation(shader_program, "y");
		GLint z_loc = glGetUniformLocation(shader_program, "z");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);
		
		glUniform1f(x_loc, position.x);
		glUniform1f(y_loc, position.y);
		glUniform1f(z_loc, position.z);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);
		
		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
	};
}

void Object::SetModel(int model_indices, int model_base_index, float model_scale = 1.0f) {
	model.n_indices = model_indices;
	model.vertex_base_index = model_base_index;
	model.scale = model_scale;
}

Model Object::GetModel() { return model; }

void Object::SetPosition(glm::vec3 new_position) { position = new_position; }

glm::vec3 Object::GetPosition() { return position; }

void Object::SetVelocity(glm::vec3 new_velocity) { velocity = new_velocity; }

glm::vec3 Object::GetVelocity() { return velocity; }

void Object::SetRotation(glm::vec3 new_rotation) { rotation = new_rotation; }

glm::vec3 Object::GetRotation() { return rotation; }

/*
bool Object::Collide(Object* target_obj) {
	bool is_x_collide = abs(target_obj->position.x - position.x) < (target_obj->collision_mask.x / 2 + collision_mask.x / 2);
	bool is_y_collide = abs(target_obj->position.y - position.y) < (target_obj->collision_mask.y / 2 + collision_mask.y / 2);
	bool is_z_collide = abs(target_obj->position.z - position.z) < (target_obj->collision_mask.z / 2 + collision_mask.z / 2);

	return (is_x_collide & is_y_collide & is_z_collide);
}
*/
void Object::StepSelf() { func[0](this); }

void Object::DrawSelf(Object* self) { func[1](this); }

// ----------- ----------- ----------- ----------- -----------

Object* MakeCarObject(ObjectList* target_obj_list, glm::vec3 make_position, int type) {
	Model make_model;
	glm::vec3 make_collision_mask;
	switch (type)
	{
	case 1:
		make_model = { 158, 38, 0.9f };
		make_collision_mask = {0.9f, 1.0f, 0.9f};
		break;
	case 2:
		make_model = { 158, 196, 0.7f };
		make_collision_mask = { 0.7f, 1.0f, 0.7f };
		break;
	default:
		make_model = { 236, 354, 0.9f };
		make_collision_mask = { 0.9f, 1.0f, 1.0f };
		break;
	}

	Object* temp_obj = new Object(make_position, 158, 38); // Manually set model of imported obj file. (158, 38) = police car, (158, 196) = taxi, (236, 354) = truck
	(*target_obj_list).CreateObject(temp_obj);
	temp_obj->SetModel(make_model.n_indices, make_model.vertex_base_index, make_model.scale);
	temp_obj->collision_mask = make_collision_mask;

	// Define step function
	temp_obj->func[0] = [](Object* self) {
		// Get player position
		glm::vec3 player_position = player_obj->GetPosition();
		
		// Calculate new position
		glm::vec3 current_position = self->GetPosition();
		glm::vec3 current_vel = self->GetVelocity();
		glm::vec3 new_position = current_position + current_vel;
		
		// Collision test
		glm::vec3 collision_mask = self->collision_mask;
		if (abs(new_position.x - player_position.x) < 1.0f) {
			bool is_x_collide = abs(player_position.x - new_position.x) < (player_obj->collision_mask.x / 2.0 + collision_mask.x / 2.0);
			bool is_z_collide = abs(player_position.z - new_position.z) < (player_obj->collision_mask.z / 2.0 + collision_mask.z / 2.0);
			if (is_x_collide & is_z_collide) {
				exit(0);
			}
		}
		// Out of border?
		if (abs(new_position.z) > room_col / 2.0f) {
			new_position.z *= -1;
		}
		// Update position
		self->SetPosition(new_position);
	};

	// Define draw function
	temp_obj->func[1] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		GLint x_loc = glGetUniformLocation(shader_program, "x");
		GLint y_loc = glGetUniformLocation(shader_program, "y");
		GLint z_loc = glGetUniformLocation(shader_program, "z");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::scale(MVP, glm::vec3(self->GetModel().scale));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		
		glUniform1f(x_loc, position.x);
		glUniform1f(y_loc, position.y);
		glUniform1f(z_loc, position.z);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);		
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};

	return temp_obj;
}

void MakeTreeObject(ObjectList* target_obj_list, glm::vec3 make_position) {

	Object* tree = new Object(make_position, 316, 590);
	(*target_obj_list).CreateObject(tree);

	// Define step function
	tree->func[0] = [](Object* self) {
		// Get player position
		glm::vec3 player_position = player_obj->GetPosition();

		// Calculate new position
		glm::vec3 new_position = self->GetPosition();

		// Collision test
		glm::vec3 collision_mask = self->collision_mask;
		if (abs(new_position.x - player_position.x) < 1.0f) {
			bool is_x_collide = abs(player_position.x - new_position.x) < (player_obj->collision_mask.x / 2.0 + collision_mask.x / 2.0);
			bool is_z_collide = abs(player_position.z - new_position.z) < (player_obj->collision_mask.z / 2.0 + collision_mask.z / 2.0);
			if (is_x_collide & is_z_collide) {
				glm::vec3 player_vel = player_obj->GetVelocity();
				player_obj->SetVelocity(glm::vec3(-player_vel.x, -player_vel.y, -player_vel.z));
			}
		}
	};

	// Define draw function
	tree->func[1] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		GLint x_loc = glGetUniformLocation(shader_program, "x");
		GLint y_loc = glGetUniformLocation(shader_program, "y");
		GLint z_loc = glGetUniformLocation(shader_program, "z");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);

		glUniform1f(x_loc, position.x);
		glUniform1f(y_loc, position.y);
		glUniform1f(z_loc, position.z);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
	};
}

Object* InitObject(ObjectList* target_obj_list) {
	// create player marker
	Object* player = new Object({ 0, 0, -0.5 }, 12, 0);
	player->collision_mask = { 0.5f, 1.0f, 0.5f };
	(*target_obj_list).CreateObject(player);

	player->func[0] = [](Object* self) {
		glm::vec3 current_position = self->GetPosition();
		glm::vec3 current_vel = self->GetVelocity();
		glm::vec3 new_position = current_position + current_vel;
		self->SetPosition(new_position);
	};

	std::srand(std::time(nullptr));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < room_col; j++) {
			Object* start_tile = new Object({ i, 0, j - room_col / 2.0 }, 2, 24);
			(*target_obj_list).CreateObject(start_tile);
		}
	}
	
	// fill grass and road
	float random = (std::rand() / 1.0) / RAND_MAX;
	for (int i = 4; i < room_row; i++) {
		int row_tile; // 26 = Grass tile, 28 = Road tile
		int temp_n_indices;
		if (random > 0.5) { // Road
			row_tile = 28;
			temp_n_indices = 10;
			random = (std::rand() / 1.0) / RAND_MAX;
			for (int j = 0; j < room_col; j++) {
				Object* road_grass = new Object({ i, 0, j - room_col / 2.0 }, temp_n_indices, row_tile);
				(*target_obj_list).CreateObject(road_grass);
			}
			float direction = (i % 2 - 0.5f);
			// create car on the road
			Object* temp = MakeCarObject(target_obj_list, { i, 0, 0 }, i % 3);
			float speed = ( std::rand() / 2.0 / RAND_MAX + 0.15 ) * direction;
			temp->SetVelocity(glm::vec3(0, 0, speed));
			temp->SetRotation(glm::vec3(0, (direction - 0.5) * 3.141592, 0));
		}
		else { // Grass
			row_tile = 26;
			temp_n_indices = 2;
			random += 0.5;

			for (int j = 0; j < room_col; j++) {
				Object* road_grass = new Object({ i, 0, j - room_col / 2.0 }, temp_n_indices, row_tile);
				(*target_obj_list).CreateObject(road_grass);
				// create tree on the road
				float random_tree = (std::rand() / 1.0) / RAND_MAX;
				if (random_tree > 0.5) {
					MakeTreeObject(target_obj_list, glm::vec3(i, 0, j - room_col / 2.0));
				}
			}
		}
		
	}

	// build wall

	return player;
}
