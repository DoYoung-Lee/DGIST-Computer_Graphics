#version 330

uniform mat4 MVP;
uniform mat4 MVP_obj;

uniform vec3 color;

in vec3 Position;

out vec3 FragColor;

void main()
{
	gl_Position = MVP * MVP_obj * vec4(Position.xyz, 1.0);
    FragColor = color;
}