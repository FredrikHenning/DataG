// This code only shows how to load an object. Input this into your
// program to also draw it.

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION 

#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr


float rotation_angles[2];
float lights;

// Read the code for the shader programs from file
std::string vertex_shader_str = readFile("../lab2-3_vs.glsl");
std::string fragment_shader_str = readFile("../lab2-3_fs.glsl");
GLuint shader_program;

// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;

	return;
}

void loadShaders(std::string vertex_shader_str,
	std::string fragment_shader_str)
{
	const char* vertex_shader_src = vertex_shader_str.c_str();
	const char* fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
}

// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		loadShaders(vertex_shader_str, fragment_shader_str);
	}

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_D) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		rotation_angles[0] += 2.0f;
	if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		rotation_angles[0] += -2.0f;
	if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		rotation_angles[1] += 2.0f;
	if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		rotation_angles[1] += -2.0f;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char** argv)
{

	std::string inputfile = "../../common/data/bunny.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	int flags = 0; // see load_flags_t enum for more information.

	// Load the object
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

	// Check for errors
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}
	else
	{
		std::cout << "Loaded " << inputfile
			<< " ("
			<< shapes.size() << " shapes"
			<< ")"
			<< "\n";
		if (shapes.size() > 0)
		{
			std::cout << shapes[0].mesh.positions.size() << " points "
				<< "(" << shapes[0].mesh.positions.size() * sizeof(float) << " bytes), "
				<< shapes[0].mesh.indices.size() << " indices "
				<< "(" << shapes[0].mesh.indices.size() * sizeof(unsigned int) << " bytes), "
				<< shapes[0].mesh.normals.size() << " normals.\n"
				;
		}
	}


	// Populate your VBO with shapes[0].mesh.positions and shapes[0].mesh.normals
	// ...
	// Populate your EBO with shapes[0].mesh.indices
	// ...
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	lights = 1;
	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	loadShaders(vertex_shader_str, fragment_shader_str);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	GLuint VAO, VBO, EBO, normalVBO;
	// Setup VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Setip VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// The new vbo for normals setup
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &shapes[0].mesh.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Setup EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(float), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);


  //-------------------------------------------------------------------------//
	
  float near = 1.0f;
  float far = 100.0f;

  while (!glfwWindowShouldClose(window))
  {
	  // Update other events like keyboard/mouse input.
	  glfwPollEvents();
	  // Clear the drawing surface before drawing something new.
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  glfwGetFramebufferSize(window, &w_width, &w_height);

	  //-----------------------------------------------------------------------//
	  // YOUR CODE GOES HERE(old)
	  // Use glm::perspective to create a projection matrix
	  glm::mat4 projectionMatrix = glm::mat4(1.0f);
	  float fov = glm::radians(90.0f);
	  projectionMatrix = glm::perspective(fov, (float(w_width) / w_height), near, far);

	  // Use glm::translate and glm::rotate to create the
	  // model and view matrices.
	  glm::mat4 modelMatrix(1.0f);
	  glm::mat4 viewMatrix(1.0f);
	  viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.5f));
	  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_angles[1]), glm::vec3(1.0f, 0.0f, 0.0f));
	  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_angles[0]), glm::vec3(0.0f, 1.0f, 0.0f));
	  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
	  modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));

	  glm::mat4 modelViewProjectionMatrix = projectionMatrix * (viewMatrix * modelMatrix);

	  int shader_location = glGetUniformLocation(shader_program, "mvpMatrix");
	  glUniformMatrix4fv(shader_location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

	  // Not sure if needed but keep just in case
	  //int modMat_location = glGetUniformLocation(shader_program, "model_matrix");
	  //glUniformMatrix4fv(modMat_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	  //

	  glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	  int modViewMat_location = glGetUniformLocation(shader_program, "model_view_matrix");
	  glUniformMatrix4fv(modViewMat_location, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

	  // Setup light values for the fragment shader
	  // Number of lights
	  glm::uint numberOfLights(lights);
	  int light_nr_location = glGetUniformLocation(shader_program, "light_count");
	  glUniform1i(light_nr_location, numberOfLights);

	  // Light position (infront of the object, no other way to get close to fig3 in LAB2.pdf)
	  glm::vec3 lightPosition = { 0.0, 0.0, 6.0 };
	  int light_position_location = glGetUniformLocation(shader_program, "light_position");
	  glUniform3fv(light_position_location, 1, glm::value_ptr(lightPosition));

	  // Light color
	  glm::vec3 lightColor = { 1.0, 1.0, 1.0 };
	  int light_color_location = glGetUniformLocation(shader_program, "light_colour");
	  glUniform3fv(light_color_location, 1, glm::value_ptr(lightColor));
	  // -----------------------------------------------------------------------//

	  //-----------------------------------------------------------------------//
	  // Call glDrawElements as before, but with the number 208353
	  // Number comes from running the program the first time and check the print
	  // and number of indices
	  glClearColor(75.0f / 255.0f, 120.0f / 255.0f, 238.0f / 255.0f, 1.0f);
	  glDrawElements(GL_TRIANGLES, 208353, GL_UNSIGNED_INT, 0);
	  
	  //-----------------------------------------------------------------------//

	  glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
  

  return 0;
}



