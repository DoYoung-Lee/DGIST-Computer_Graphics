#version 330

uniform mat4 MVP;

uniform float x;
uniform float y;
uniform float z;
uniform float Scale;

in vec3 Position;
in vec3 Color;

out vec3 FragColor;

void main()
{
	vec3 xyz = Scale * Position.xyz + vec3(x,y,z);
	gl_Position = MVP * vec4(xyz, 1.0);
    FragColor = Color;
}