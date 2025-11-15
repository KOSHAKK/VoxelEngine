#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 t_coords;

uniform mat4 MVP;

uniform float time;



void main()
{
	t_coords = texture_coords;
	gl_Position = MVP * vec4(vertex_position, 1.0);
}