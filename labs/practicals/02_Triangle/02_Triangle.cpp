#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;

bool load_content() {
  // Create triangle data
  // Positions
  vector<vec3> positions{
      // *********************************
      // Add the three positions of the triangle corners here

      // *********************************

	  vec3(0.0, 1.0 ,0.0),
	  vec3(-1.0, -1.0, 0.0),
	  vec3(1.0, -1.0, 0.0)

  };
  // Colours
  vector<vec4> colours{vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(100.0f, 30.0f, 90.0f, 12.0f), vec4(0.0f, 350.0f, 0.0f, 1.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  // Load in shaders
  eff.add_shader("shaders/basic.vert", // filename
                 GL_VERTEX_SHADER);    // type
  eff.add_shader("shaders/basic.frag", // filename
                 GL_FRAGMENT_SHADER);  // type
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
                     1,                               // Number of values - 1 mat4
                     GL_FALSE,                        // Transpose the matrix?
                     value_ptr(MVP));                 // Pointer to matrix data
  // Render geometry
  renderer::render(geom);
  return true;
}

void main() {
  // Create application
  app application("02_Triangle");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}