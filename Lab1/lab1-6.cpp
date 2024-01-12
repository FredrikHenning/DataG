
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

// Read the code for the shader programs from file
std::string vertex_shader_str =	readFile("../lab1-6_vs.glsl");
std::string fragment_shader_str =	readFile("../lab1-6_fs.glsl");
GLuint shader_program;


// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
	if(isCompiled == GL_FALSE)
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


// Load vertex and fragment shader on demand, and recompile the shader program
void loadShaders( std::string vertex_shader_str,
                  std::string fragment_shader_str )
{
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader_src, NULL);
	glCompileShader (vs);
	checkShaderCompileError(vs);
  
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader_src, NULL);
	glCompileShader (fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram ();
	glAttachShader (shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram (shader_program);
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
	//-------------------------------------------------------------------------//

	//  if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
	//  if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )     
	//  if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
	//  if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




int main(int argc, char const *argv[])
{
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow (w_width, w_height, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	glewExperimental = GL_TRUE;
	glewInit ();

	loadShaders( vertex_shader_str, fragment_shader_str );
	
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

	//-------------------------------------------------------------------------//
	// COPY FROM lab1-5
	// Geometry, VBO, EBO, VAO
	float points[] = {
						-0.5f, -0.5f, -0.5f, 1.0f,		//vertex 0
						0.5f, -0.5f, -0.5f,	 1.0f,	    //vertex 1
						0.5f, 0.5f, -0.5f,	 1.0f,	    //vertex 2
						-0.5f, 0.5f, -0.5f,  1.0f,	    //vertex 3
						-0.5f, -0.5f, 0.5f,  1.0f,	    //vertex 4
						0.5f, -0.5f, 0.5f,	 1.0f,	    //vertex 5
						0.5f, 0.5f, 0.5f,	 1.0f,	    //vertex 6
						-0.5f, 0.5f, 0.5f,	 1.0f	    //vertex 7
	};

	unsigned short faces[] = {
								0, 1, 2,	//triangle 0
								0, 2, 3,	//triangle 1
								4, 5, 1,	//triangle 2
								4, 1, 0,	//triangle 3
								5, 4, 7, 	//triangle 4
								5, 7, 6,	//triangle 5
								3, 2, 6,	//triangle 6
								3, 6, 7,	//triangle 7
								4, 0, 3,	//triangle 8
								4, 3, 7,	//triangle 9
								1, 5, 6,	//triangle 10
								1, 6, 2		//triangle 11
	};



	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//-------------------------------------------------------------------------//
	
	float near = 1.0f;
	float far = 100.0f;

	while (!glfwWindowShouldClose (window)) 
	{
		// Update other events like keyboard/mouse input.
		glfwPollEvents ();
		// Clear the drawing surface before drawing something new.
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize(window, &w_width , &w_height );

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		//
		// Replace your hand-crafted matrices from lab1-5.cpp with ones
		// created by glm.
		//
		// Use glm::perspective to create a projection matrix
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		float fov = glm::radians(90.0f);
		projectionMatrix = glm::perspective(fov, (float(w_width) / w_height), near, far);

		//
		// Use glm::translate and glm::rotate to create the
		// model and view matrices.

		glm::mat4 modelMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_angles[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_angles[0]), glm::vec3(0.0f, 1.0f, 0.0f));

		//
		// Multiply your matrices in the correct order to get a
		// modelViewProjection matrix and upload it to the appropriate
		// uniform variable in vertex shader.
		glm::mat4 modelViewProjectionMatrix = projectionMatrix * (viewMatrix * modelMatrix);

		int shader_location = glGetUniformLocation(shader_program, "position");
		glUniformMatrix4fv(shader_location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
		// -----------------------------------------------------------------------//
	 
		//-----------------------------------------------------------------------//
		// Call glDrawElements as before
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

