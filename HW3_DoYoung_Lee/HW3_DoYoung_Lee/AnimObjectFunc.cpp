/* -------------------------------- 
	References:
	Referenced Jinwoo Yu's computer graphics assignment 1 for Object class parts.
	Referenced Prof. Sunghyun Cho's example code 9 for matrices stack part.
   -------------------------------- */

#include "AnimObjectFunc.h"
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Matrix stack			--------	--------

//	--------	--------	--------	--------	--------	--------

MatrixStack::MatrixStack(int _max_size) {
	max_size = _max_size;
}

MatrixStack::~MatrixStack() {
	
}

void MatrixStack::PushMatrix(glm::mat4 _MVP) {
	assert(matrices_list.size() + 1 < max_size);
	matrices_list.push_back(_MVP);
}

glm::mat4 MatrixStack::PopMatrix() {
	assert(matrices_list.size() - 1 >= 0);
	glm::mat4 out_MVP = matrices_list.back();
	matrices_list.pop_back();
	return out_MVP;
}

MatrixStack mvp_stack(16);

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Default Object			--------	--------

//	--------	--------	--------	--------	--------	--------

Object::Object(glm::vec3 _position, int _n_indices, int _vertex_base_index) {
	position = _position;
	velocity = { 0, 0, 0 };
	rotation = { 0, 0, 0 };
	model.n_indices = _n_indices;
	model.vertex_base_index = _vertex_base_index;
	model.scale = 1.0f;
	collision_mask = {0.0f, 0.0f, 0.0f};

	func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		/* Step function of self*/

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	func[Draw] = [](Object* self) {
		std::vector<Object*>* child_list = self->GetChildrenList();
		int num_child = child_list->size();
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Draw]((*child_list)[i]);
		}
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

void Object::SetColor(glm::vec3 _color) {
	color = _color;
}

glm::vec3 Object::GetColor() { return color; }

void Object::AppendChild(Object* _object) { children_list.push_back(_object); }

std::vector<Object*>* Object::GetChildrenList() { return &children_list; }

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Tile object				--------	--------

//	--------	--------	--------	--------	--------	--------

Object* MakeTileObject(Object* _obj_list, glm::vec3 _position, int _type) {
	// create player marker
	Object* tile = new Object(_position, 0, 0);
	Model make_model = object_model_map["tile"];
	tile->SetModel(make_model.n_indices, make_model.vertex_base_index, make_model.scale);
	tile->collision_mask = { 0.0f, 0.0f, 0.0f };
	// type 0 = white, type 1 = start/end, type 2 = grass, type 3 = road
	switch (_type) {
	default:
		tile->SetColor({ 1.0f, 1.0f, 1.0f });
		break;
	case 1:
		tile->SetColor({0.0f, 0.0f, 1.0f});
		break;
	case 2:
		tile->SetColor({0.0f, 1.0f, 0.0f});
		break;
	case 3:
		tile->SetColor({ 0.2f, 0.2f, 0.2f });
		break;
	}
	
	tile->func[Step] = [](Object* self) {};

	tile->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::scale(MVP, glm::vec3(self->GetModel().scale));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));

		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
	};

	_obj_list->AppendChild(tile);

	return tile;
}


//	--------	--------	--------	--------	--------	--------

//	--------	--------	Car object				--------	--------

//	--------	--------	--------	--------	--------	--------

Object* MakeCarObject(Object* _obj_list, glm::vec3 _position, int _type) {
	Model make_model;
	glm::vec3 make_collision_mask;
	switch (_type) {
	case 1:
		make_model = object_model_map["police_car"];
		make_model.scale = 0.9f;
		make_collision_mask = {0.85f, 1.0f, 0.9f};
		break;
	case 2:
		make_model = object_model_map["taxi"];
		make_model.scale = 0.7f;
		make_collision_mask = { 0.65f, 1.0f, 0.7f };
		break;
	default:
		make_model = object_model_map["truck"];
		make_model.scale = 0.9f;
		make_collision_mask = { 0.85f, 1.0f, 1.0f };
		break;
	}

	Object* temp_obj = new Object(_position, 0, 0);
	temp_obj->SetModel(make_model.n_indices, make_model.vertex_base_index, make_model.scale);
	temp_obj->collision_mask = make_collision_mask;
	
	// Define step function
	temp_obj->func[Step] = [](Object* self) {
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
			if (is_x_collide && is_z_collide) {
				exit(0);
			}
		}
		// Out of border?
		if (abs(new_position.z) > room_col / 2.0f + 1) {
			new_position.z *= -1;
		}
		// Update position
		self->SetPosition(new_position);
	};

	// Define draw function
	temp_obj->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");
		
		glm::mat4 MVP = glm::mat4(1.0f);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::scale(MVP, glm::vec3(model.scale));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);		
		glUniform3f(color_loc, 1.0f, 1.0f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};
	_obj_list->AppendChild(temp_obj);
	return temp_obj;
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Tree object				--------	--------

//	--------	--------	--------	--------	--------	--------

void MakeTreeObject(Object* _obj_list, glm::vec3 _position) {
	Object* tree = new Object(_position, 0, 0);
	Model make_model = object_model_map["christmas_tree"];
	tree->SetModel(make_model.n_indices, make_model.vertex_base_index, 0.8f);
	tree->collision_mask = glm::vec3(0.8f, 0.8f, 0.8f);
	tree->SetColor({0.5f, 0.5f, 0.5f});

	// Define step function
	tree->func[Step] = [](Object* self) {
		// Get player position
		glm::vec3 player_position = player_obj->GetPosition();

		// Calculate new position
		glm::vec3 new_position = self->GetPosition();

		// Collision test
		glm::vec3 collision_mask = self->collision_mask;
		if (abs(new_position.x - player_position.x) < 1.0f) {
			bool is_x_collide = abs(player_position.x - new_position.x) < (player_obj->collision_mask.x / 2.0 + collision_mask.x / 2.0);
			bool is_z_collide = abs(player_position.z - new_position.z) < (player_obj->collision_mask.z / 2.0 + collision_mask.z / 2.0);
			if (is_x_collide && is_z_collide) {
				// Set destination to previous cell
				player_destination = new_position + glm::vec3(-move_axis[0], 0, -move_axis[1]);
			}
		}
	};

	// Define draw function
	tree->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::scale(MVP, glm::vec3(self->GetModel().scale));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));

		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
	};
	_obj_list->AppendChild(tree);
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Wall Object				--------	--------

//	--------	--------	--------	--------	--------	--------

void MakeWallObject(Object* _obj_list, glm::vec3 _position) {

	Object* wall = new Object(_position, 0, 0);
	wall->collision_mask = glm::vec3(1.0f, 1.0f, 1.0f);
	// Define step function
	wall->func[Step] = [](Object* self) {
		// Get player position
		glm::vec3 player_position = player_obj->GetPosition();

		// Calculate new position
		glm::vec3 new_position = self->GetPosition();

		// Collision test
		glm::vec3 collision_mask = self->collision_mask;
		if (abs(new_position.x - player_position.x) < 1.0f) {
			bool is_x_collide = abs(player_position.x - new_position.x) < (player_obj->collision_mask.x / 2.0 + collision_mask.x / 2.0);
			bool is_z_collide = abs(player_position.z - new_position.z) < (player_obj->collision_mask.z / 2.0 + collision_mask.z / 2.0);
			if (is_x_collide && is_z_collide) {
				// Set destination to previous cell
				player_destination = new_position + glm::vec3(-move_axis[0], 0, -move_axis[1]);
			}
		}
	};

	// Define draw function
	wall->func[Draw] = [](Object* self) {/* Do not draw wall objects */};
	(*_obj_list).AppendChild(wall);
}

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Player Object			--------	--------

//	--------	--------	--------	--------	--------	--------

Object* MakePlayerObject(Object* _obj_list, glm::vec3 _position) {
	Model make_model = object_model_map["torso"];
	Object* player = new Object(_position, make_model.n_indices, make_model.vertex_base_index);
	player->collision_mask = { 0.5f, 1.0f, 0.5f };
	player->SetColor({ 1.0f, 0.0f, 0.0f });

	// Create children
	Object* left_arm = LeftArm(player);
	LeftLowerArm(left_arm);
	Object* right_arm = RightArm(player);
	RightLowerArm(right_arm);

	player->func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		/* Step function of self*/
		glm::vec3 current_position = self->GetPosition();
		glm::vec3 current_vel = self->GetVelocity();
		glm::vec3 new_position = current_position + current_vel * (player_destination - current_position);
		if (abs(player_destination - current_position).x < 0.0625f && abs(player_destination - current_position).z < 0.0625)
			new_position = player_destination;
		self->SetPosition(new_position);

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	player->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = glm::mat4(1.0f);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = glm::scale(MVP, glm::vec3(self->GetModel().scale));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));

		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
		
		auto children = self->GetChildrenList();
		mvp_stack.PushMatrix(MVP);
		for (int i = 0; i < children->size(); i++) {
			(*children)[i]->func[Draw]((*children)[i]);
		}
		mvp_stack.PopMatrix();
	};

	_obj_list->AppendChild(player);
	return player;
}

Object* LeftArm(Object* _obj_list) {
	Model make_model = object_model_map["arm"];
	Object* left_arm = new Object({ 0.0, 0.35, -0.3 }, make_model.n_indices, make_model.vertex_base_index);
	left_arm->collision_mask = { 0.0f, 0.0f, 0.0f };
	left_arm->SetColor({ 0.8f, 0.0f, 0.8f });

	left_arm->func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		// Make arm rotation animate
		glm::vec3 current_position = player_obj->GetPosition();
		bool distance_left = (player_destination != current_position);
		self->SetRotation({ 0, 0, PI - distance_left * 0.125 * PI });

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	left_arm->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = mvp_stack.PopMatrix();
		mvp_stack.PushMatrix(MVP);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));
		
		auto children = self->GetChildrenList();
		mvp_stack.PushMatrix(MVP);
		for (int i = 0; i < children->size(); i++) {
			(*children)[i]->func[Draw]((*children)[i]);
		}
		mvp_stack.PopMatrix();
	};

	_obj_list->AppendChild(left_arm);
	return left_arm;
};

Object* LeftLowerArm(Object* _obj_list) {
	Model make_model = object_model_map["arm"];
	Object* left_lower_arm = new Object({ 0.0, 0.2, 0.0 }, make_model.n_indices, make_model.vertex_base_index);
	left_lower_arm->collision_mask = { 0.0f, 0.0f, 0.0f };
	left_lower_arm->SetColor({ 0.8f, 0.8f, 0.8f });

	left_lower_arm->func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		// Make arm rotation animate
		glm::vec3 current_position = player_obj->GetPosition();
		bool distance_left = (player_destination != current_position);
		self->SetRotation({ 0, 0, - distance_left * 0.125 * PI });

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	left_lower_arm->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = mvp_stack.PopMatrix();
		mvp_stack.PushMatrix(MVP);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));

		auto children = self->GetChildrenList();
		mvp_stack.PushMatrix(MVP);
		for (int i = 0; i < children->size(); i++) {
			(*children)[i]->func[Draw];
		}
		mvp_stack.PopMatrix();
	};

	_obj_list->AppendChild(left_lower_arm);
	return left_lower_arm;
};

Object* RightArm(Object* _obj_list) {
	Model make_model = object_model_map["arm"];
	Object* right_arm = new Object({ 0.0, 0.35, 0.3 }, make_model.n_indices, make_model.vertex_base_index);
	right_arm->collision_mask = { 0.0f, 0.0f, 0.0f };
	right_arm->SetColor({ 0.8f, 0.0f, 0.8f });

	right_arm->func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		// Make arm rotation animate
		glm::vec3 current_position = player_obj->GetPosition();
		bool distance_left = (player_destination != current_position);
		self->SetRotation({ 0, 0, PI + distance_left * 0.125 * PI });

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	right_arm->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = mvp_stack.PopMatrix();
		mvp_stack.PushMatrix(MVP);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));

		auto children = self->GetChildrenList();
		mvp_stack.PushMatrix(MVP);
		for (int i = 0; i < children->size(); i++) {
			(*children)[i]->func[Draw]((*children)[i]);
		}
		mvp_stack.PopMatrix();
	};

	_obj_list->AppendChild(right_arm);
	return right_arm;
};

Object* RightLowerArm(Object* _obj_list) {
	Model make_model = object_model_map["arm"];
	Object* right_lower_arm = new Object({ 0.0, 0.2, 0.0 }, make_model.n_indices, make_model.vertex_base_index);
	right_lower_arm->collision_mask = { 0.0f, 0.0f, 0.0f };
	right_lower_arm->SetColor({ 0.8f, 0.8f, 0.8f });

	right_lower_arm->func[Step] = [](Object* self) {
		std::vector<Object*> *child_list = self->GetChildrenList();
		int num_child = child_list->size();

		// Make arm rotation animate
		glm::vec3 current_position = player_obj->GetPosition();
		bool distance_left = (player_destination != current_position);
		self->SetRotation({ 0, 0, distance_left * 0.125 * PI });

		// Step function of children
		for (int i = 0; i < num_child; i++) {
			(*child_list)[i]->func[Step]((*child_list)[i]);
		}
	};

	right_lower_arm->func[Draw] = [](Object* self) {
		glm::vec3 position = self->GetPosition();
		Model model = self->GetModel();
		glm::vec3 v_color = self->GetColor();
		GLint color_loc = glGetUniformLocation(shader_program, "color");
		GLint matrix_loc = glGetUniformLocation(shader_program, "MVP_obj");

		glm::mat4 MVP = mvp_stack.PopMatrix();
		mvp_stack.PushMatrix(MVP);
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		MVP = glm::rotate(MVP, self->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = glm::rotate(MVP, self->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = glm::translate(MVP, glm::vec3(-position.x, -position.y, -position.z));
		MVP = glm::translate(MVP, glm::vec3(position.x, position.y, position.z));
		glUniform3f(color_loc, v_color.r, v_color.g, v_color.b);
		glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, &MVP[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * model.n_indices, GL_UNSIGNED_INT, reinterpret_cast<void*> (model.vertex_base_index * sizeof(glm::vec3)));

		auto children = self->GetChildrenList();
		mvp_stack.PushMatrix(MVP);
		for (int i = 0; i < children->size(); i++) {
			(*children)[i]->func[Draw];
		}
		mvp_stack.PopMatrix();
	};

	_obj_list->AppendChild(right_lower_arm);
	return right_lower_arm;
};

//	--------	--------	--------	--------	--------	--------

//	--------	--------	Object Initialization	--------	--------

//	--------	--------	= Make Map				--------	--------

//	--------	--------	--------	--------	--------	--------


Object* InitObject(Object* _obj_list) {
	Object* player = MakePlayerObject(_obj_list, {0.0f, 0.0f, 0.5f});

	std::srand(std::time(nullptr));
	for (int j = 0; j < room_col + 2; j++) {
		MakeWallObject(_obj_list, { -1.0, 0, j - (room_col + 2.0) / 2.0 });
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < room_col; j++) {
			//Object* start_tile = new Object({ i, 0, j - room_col / 2.0 }, 2, 24);
			MakeTileObject(_obj_list, { i, 0, j - room_col / 2.0 }, 1);
		}
		MakeWallObject(_obj_list, { i, 0, -(room_col / 2.0 + 1) });
		MakeWallObject(_obj_list, { i, 0, room_col / 2.0 });
	}
	
	// fill grass and road
	float random = (std::rand() / 1.0) / RAND_MAX;
	for (int i = 4; i < room_row; i++) {
		int row_tile;
		int temp_n_indices;
		if (random > 0.5) { // Road
			row_tile = 28;
			temp_n_indices = 10;
			random = (std::rand() / 1.0) / RAND_MAX;
			for (int j = 0; j < room_col; j++) {
				//Object* road_grass = new Object({ i, 0, j - room_col / 2.0 }, temp_n_indices, row_tile);
				MakeTileObject(_obj_list, { i, 0, j - room_col / 2.0 }, 3);
			}
			float direction = (i % 2 - 0.5f);
			// create car on the road
			Object* temp = MakeCarObject(_obj_list, { i, 0, 0 }, i % 3);
			float speed = ( std::rand() / 2.0 / RAND_MAX + 0.15 ) * direction;
			temp->SetVelocity(glm::vec3(0, 0, speed));
			temp->SetRotation(glm::vec3(0, (direction - 0.5) * 3.141592, 0));
		}
		else { // Grass
			row_tile = 26;
			temp_n_indices = 2;
			random += 0.5;

			for (int j = 0; j < room_col; j++) {
				//Object* road_grass = new Object({ i, 0, j - room_col / 2.0 }, temp_n_indices, row_tile);
				MakeTileObject(_obj_list, { i, 0, j - room_col / 2.0 }, 2);
				// create tree on the road
				float random_tree = (std::rand() / 1.0) / RAND_MAX;
				if (random_tree > 0.6) {
					MakeTreeObject(_obj_list, glm::vec3(i, 0, j - room_col / 2.0));
				}
			}
		}
		MakeWallObject(_obj_list, { i, 0, -(room_col / 2.0 + 1) });
		MakeWallObject(_obj_list, { i, 0, room_col / 2.0 });
	}

	// End tile
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < room_col; j++) {
			//Object* end_tile = new Object({ room_row+i, 0, j - room_col / 2.0 }, 2, 24);
			Object* end_tile = MakeTileObject(_obj_list, { room_row + i, 0, j - room_col / 2.0 }, 1);
			end_tile->func[Step] = [](Object* self) {
				if (player_obj->GetPosition().x > self->GetPosition().x)
					exit(0);
			};
		}
		MakeWallObject(_obj_list, { i, 0, -(room_col / 2.0 + 1) });
		MakeWallObject(_obj_list, { i, 0, room_col / 2.0 });
	}

	return player;
}


