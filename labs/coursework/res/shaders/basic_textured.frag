#version 410

// Define sampler used to get texture colour
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
layout (location = 0) in vec3 vertex_colour;

// Outgoing colour
layout (location = 0) out vec4 out_colour;

void main()
{
	//sets the out colour to be the texture with the ambiant light added to it
	out_colour = vertex_colour * texture(tex, tex_coord);
}
