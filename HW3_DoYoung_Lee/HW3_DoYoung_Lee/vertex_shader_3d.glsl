#version 330

uniform mat4 MVP;
uniform mat4 MVP_obj;

uniform float x;
uniform float y;
uniform float z;

in vec3 Position;
in vec3 Color;

out vec3 FragColor;

void main()
{
	vec3 xyz = Position.xyz + vec3(x,y,z);
	gl_Position = MVP * MVP_obj * vec4(xyz, 1.0);
    FragColor = Color;
}