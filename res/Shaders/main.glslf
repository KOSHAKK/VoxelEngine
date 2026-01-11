#version 460

in vec4 a_color;
in vec2 a_texCoord;
out vec4 f_color;

uniform sampler2D u_texture0;

void main(){
	f_color = a_color * texture(u_texture0, a_texCoord);
	//f_color = vec4(1.f, 0.f, 0.f, 1.f);
	//f_color = a_color;
}
