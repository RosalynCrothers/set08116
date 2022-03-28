#include <glm\glm.hpp>
#include <graphics_framework.h>

#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

//initialises plane and plane texture meshes
mesh plane;
texture plane_tex;

// initialises objects mesh and textures arrays
std::array<mesh, 4> meshes;
std::array<texture, 4> texs;

// initialises geom and effect
geometry geom;
effect eff;

//keep track of current camera, set to fixed at first
int c_cam = 1;

//initialise cameras
target_camera cam;
free_camera cam2;

//initialise cursor location to 0
double cursor_x = 0.0;
double cursor_y = 0.0;


bool initialise() {
	// Sets input mode, hides cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// get initial cursor position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);


	return true;
}


bool load_content() {
	// Create triangle data
	vector<vec3> positions{ vec3(0.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f)

	};
	// Colours
	vector<vec4> colours{ vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	// Add to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);


	//creates the plane
	plane = mesh(geometry_builder::create_plane());

	//creates 4 objects
	meshes[0] = mesh(geometry_builder::create_pyramid(vec3(3, 3, 3)));
	meshes[1] = mesh(geometry_builder::create_box(vec3(3, 3, 3)));
	meshes[2] = mesh(geometry_builder::create_torus(10, 10, 10));
	meshes[3] = mesh(geometry("res/models/eye.obj"));

	//transforms the plane
	plane.get_transform().scale = vec3(5.0f, 1.0f, 5.0f);

	//transforms the blue pyramid
	meshes[0].get_transform().scale = vec3(20.0f, 20.0f, 20.0f);
	meshes[0].get_transform().translate(vec3(5.0f, 90.0f, 10.0f));

	//transforms the box
	meshes[1].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	meshes[1].get_transform().translate(vec3(0.0f, -2.99f, 0.0f));

	//transforms the torus
	meshes[2].get_transform().scale = vec3(0.1f, 0.1f, 0.1f);
	meshes[2].get_transform().translate(vec3(-10.0f, 2.0f, -10.0f));

	//transforms the eye object
	meshes[3].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
	meshes[3].get_transform().translate(vec3(-20.0f, 0.0f, 60.0f));


	// Load in shaders
	eff.add_shader("res/shaders/basic_textured.vert", GL_VERTEX_SHADER);
	eff.add_shader("res/shaders/basic_textured.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();

	plane_tex = texture("res/textures/sahara_up.jpg");
	texs[0] = texture("res/textures/sahara_up.jpg");
	texs[1] = texture("res/textures/check_1.png");
	texs[2] = texture("res/textures/Purple.png");
	texs[3] = texture("res/textures/eye/eye_D.jpg");

	// Set fixed camera properties
	cam.set_position(vec3(80.0f, 100.0f, 100.0f));
	cam.set_target(vec3(-50.0f, 2.5f, 10.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	// sets free cam properties
	cam2.set_position(vec3(80.0f, 100.0f, 100.0f));
	cam2.set_target(vec3(-50.0f, 2.5f, 10.0f));
	cam2.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	return true;
}


bool update(float delta_time) {


		//gets screen height and width, then the ratio of them
		static const float sh = static_cast<float>(renderer::get_screen_height());
		static const float sw = static_cast<float>(renderer::get_screen_height());
		static const double ratio_width = quarter_pi<float>() / sw;
		static const double ratio_height = (quarter_pi<float>() * (sh / sw)) / sh;

		//creates current positions, empty to begin with
		double current_x;
		double current_y;

		//gets current cursor position
		glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

		//gets current cursor location in relation to previous one
		double delta_x = current_x - cursor_x;
		double delta_y = current_y - cursor_y;

		//gets deltas based off ratio
		delta_x = delta_x * ratio_width;
		delta_y = delta_y * ratio_height;

		//rotates camera as needed
		cam2.rotate(delta_x, -delta_y);


		//sets the cursor location
		cursor_x = current_x;
		cursor_y = current_y;


		if (glfwGetKey(renderer::get_window(), GLFW_KEY_1)) {
			c_cam = 1;
		}


		// 2 - (-50, 10, 50)
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_2)) {
			c_cam = 2;
		}

		//sets movespeed to be a constant, makes it easier to edit
		float movespeed = 0.5f;


		// allows you to press shift to change move speed by 4x
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_SHIFT)) {
			movespeed = movespeed + 1.5f;
		}

		//camera controls - wasd to move aroud, space to go up, left control to go down
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
			cam2.move(vec3(0.0f, 0.0f, movespeed));
		}
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
			cam2.move(vec3(-movespeed, 0.0f, 0.0f));
		}
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
			cam2.move(vec3(0.0f, 0.0f, -movespeed));
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
			cam2.move(vec3(movespeed, 0.0f, 0.0f));
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_SPACE)) {
			cam2.move(vec3(0.0f, movespeed, 0.0f));
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_CONTROL)) {
			cam2.move(vec3(0.0f, -movespeed, 0.0f));
		}

		mesh &target_mesh = meshes[3];
		// rotate the triangle ontop of the cube

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
			target_mesh.get_transform().rotate(vec3(0.0f, 0.2f, 0.0f));
		}
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
			target_mesh.get_transform().rotate(vec3(0.0f, -0.2f, 0.0f));
		}
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
			target_mesh.get_transform().rotate(vec3(0.2f, 0.0f, 0.0f));
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
			target_mesh.get_transform().rotate(vec3(-0.2f, 0.0f, 0.0f));
		}

	// Camera updates
	cam.update(delta_time);
	cam2.update(delta_time);
	return true;
}

bool render() {

	//switch statement to allow for multiple cameras
	switch (c_cam)
	{
	case 1:
		renderer::bind(eff);
		// Get PV (note its set to auto for case 1, but not 2 to stop from breaking)
		auto PV = cam.get_projection() * cam.get_view();

		//set textures
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Render meshes using a loop
		for (size_t i = 0; i < meshes.size(); i++) {

			auto M = meshes[i].get_transform().get_transform_matrix();

			//gets the M for each mesh
			for (size_t j = i; j > 0; j--) {
				M = meshes[j - 1].get_transform().get_transform_matrix() * M;
			}

			// Set MVP 
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * M));

			// Set material colour to yellow
			glUniform4fv(eff.get_uniform_location("material_colour"), 1, value_ptr(vec4(1, 0, 0, 1)));

			// Set ambient intensity to full
			glUniform4fv(eff.get_uniform_location("ambient_intensity"), 1, value_ptr(vec4(1, 1, 1, 1.0)));

			// Bind texture to renderer
			renderer::bind(texs[i], 0);

			// Render current mesh
			renderer::render(meshes[i]);
		}

		// Render floor plane
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * plane.get_transform().get_transform_matrix()));

		//sets colour of ambient lighting to yellow
		glUniform4fv(eff.get_uniform_location("material_colour"), 1, value_ptr(vec4(1, 0, 0, 1)));
		
		// Set ambient intensity to full
		glUniform4fv(eff.get_uniform_location("ambient_intensity"), 1, value_ptr(vec4(1, 1, 1, 1.0)));

		// Bind floor texture
		renderer::bind(plane_tex, 0);

		// Render floor
		renderer::render(plane);
		return true;

	case 2:

		renderer::bind(eff);

		// Get PV (note its set to auto for case 1, but not 2 to stop from breaking)
		PV = cam2.get_projection() * cam2.get_view();

		//set textures
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Render meshes using a loop
		for (size_t i = 0; i < meshes.size(); i++) {

			auto M = meshes[i].get_transform().get_transform_matrix();

			//gets the M for each mesh
			for (size_t j = i; j > 0; j--) {
				M = meshes[j - 1].get_transform().get_transform_matrix() * M;
			}

			// Set MVP 
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * M));

			// Set material colour - all yellow
			glUniform4fv(eff.get_uniform_location("material_colour"), 1, value_ptr(vec4(1, 0, 0, 1)));

			// Set ambient intensity to full
			glUniform4fv(eff.get_uniform_location("ambient_intensity"), 1, value_ptr(vec4(1, 1, 1, 1.0)));

			// Bind texture to renderer
			renderer::bind(texs[i], 0);

			// Render current mesh
			renderer::render(meshes[i]);
		}

		// Render floor
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * plane.get_transform().get_transform_matrix()));

		//sets colour of ambient lighting to yellow
		glUniform4fv(eff.get_uniform_location("material_colour"), 1, value_ptr(vec4(1, 0, 0, 1)));
		// Set ambient intensity to full
		glUniform4fv(eff.get_uniform_location("ambient_intensity"), 1, value_ptr(vec4(1, 1, 1, 1.0)));

		// Bind floor texture
		renderer::bind(plane_tex, 0);

		// Render floor
		renderer::render(plane);
		return true;
	}
	return true;
}

void main() {
	// Create application
	app application("Graphics Coursework");
	// Set load content, initialiseation, update and render methods
	application.set_load_content(load_content);
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}