
class AnimObject {
private:
	float x, y, z;
	float vel_x, vel_y, xel_z;
	int n_vertices;
	int vertex_base_index;

public:
	float GetPosition();
	void SetPosition(int, int, int);

};

float AnimObject::GetPosition() {
	return x, y, z;
}

void AnimObject::SetPosition(int new_x, int new_y, int new_z) {
	x = new_x;
	y = new_y;
	z = new_z;
}