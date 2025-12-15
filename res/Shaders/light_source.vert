#version 460

layout(location = 0) in vec3 vertex_position;

uniform mat4 MVP;

uniform float time;


void main()
{
	gl_Position = MVP * vec4(vertex_position, 1.0);
}