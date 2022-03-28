#version 410

// Model view projection matrix
uniform mat4 MVP;

// defines ambient intensity and material colour
uniform vec4 ambient_intensity;
uniform vec4 material_colour;


//takes in positions and texture coords
layout (location = 0) in vec3 position;
layout (location = 10) in vec2 tex_coord_in;

//returns texture coords, and ambient shading
layout (location = 0) out vec2 tex_coord;
layout(location = 10) out vec4 vertex_colour;

void main()
{
	// Calculates screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);


	// Output tex coord to fragement shader
	tex_coord = tex_coord_in;

	//set up ambient variable
	vec4 ambientTemp = ambient_intensity * material_colour;
	//output ambient temperature to fragment shader
	vertex_colour = ambientTemp;
}
