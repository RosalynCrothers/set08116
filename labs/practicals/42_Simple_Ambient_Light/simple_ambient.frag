#version 410

// Sampler used to get texture colour
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;

layout (location = 10) in vec4 ambient
// Outgoing colour
layout (location = 0) out vec4 out_colour;

void main()
{
	// ****************************************
	// Set out colour to sampled texture colour
	// ****************************************
	out_colour = ambient * texture(tex, tex_coord);
}
