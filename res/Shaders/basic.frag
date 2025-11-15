#version 460

in vec2 t_coords;

out vec4 frag_color;

uniform sampler2D tex;


uniform float time;

void main()
{
	vec2 tex_coords = t_coords;

	frag_color = texture2D(tex, tex_coords);

	

}