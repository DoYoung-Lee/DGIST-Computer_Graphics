#version 330

//layout(location = 1) out vec4 FragColor;
//layout(location = 0) out vec4 FragColor2;

in vec3 FragColor;

out vec4 color;

void main()
{
    color = vec4(FragColor.r, FragColor.g, FragColor.b, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}
