#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "lodepng.h"

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
std::string vertex_shader_str = readFile("../lab3-2_vs.glsl");
std::string fragment_shader_str = readFile("../lab3-2_fs.glsl");
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


// Load vertex and fragment shader on demand, and recompile the shader program
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
        rotation_angles[0] += 0.5f;
    if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        rotation_angles[0] += -0.5f;
    if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        rotation_angles[1] += 0.5f;
    if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        rotation_angles[1] += -0.5f;
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



// Subtracting two arrays (A - B) and put the result in array in C
static void subArrays(float* C, float* B, float* A) {
    for (int i = 0; i < 3; i++) {
        C[i] = A[i] - B[i];
    }
};

int main(int argc, char const* argv[])
{

    float t = (1.0f + sqrtf(5.0f)) * 0.25f;
    int num_faces = 20;
    int num_verts = 60;

    float points[] = {
        // An icosahedron without shared vertices has 20*3 vertices
        -0.5,  t,  0,
        -0.5,  t,  0,
        -0.5,  t,  0,
        -0.5,  t,  0,
        -0.5,  t,  0,

        0.5,  t,  0,
        0.5,  t,  0,
        0.5,  t,  0,
        0.5,  t,  0,
        0.5,  t,  0,

        -0.5, -t,  0,
        -0.5, -t,  0,
        -0.5, -t,  0,
        -0.5, -t,  0,
        -0.5, -t,  0,

        0.5, -t,  0,
        0.5, -t,  0,
        0.5, -t,  0,
        0.5, -t,  0,
        0.5, -t,  0,

        0, -0.5,  t,
        0, -0.5,  t,
        0, -0.5,  t,
        0, -0.5,  t,
        0, -0.5,  t,

        0,  0.5,  t,
        0,  0.5,  t,
        0,  0.5,  t,
        0,  0.5,  t,
        0,  0.5,  t,

        0, -0.5, -t,
        0, -0.5, -t,
        0, -0.5, -t,
        0, -0.5, -t,
        0, -0.5, -t,

        0,  0.5, -t,
        0,  0.5, -t,
        0,  0.5, -t,
        0,  0.5, -t,
        0,  0.5, -t,

        t,  0, -0.5,
        t,  0, -0.5,
        t,  0, -0.5,
        t,  0, -0.5,
        t,  0, -0.5,

        t,  0,  0.5,
        t,  0,  0.5,
        t,  0,  0.5,
        t,  0,  0.5,
        t,  0,  0.5,

        -t,  0, -0.5,
        -t,  0, -0.5,
        -t,  0, -0.5,
        -t,  0, -0.5,
        -t,  0, -0.5,

        -t,  0,  0.5,
        -t,  0,  0.5,
        -t,  0,  0.5,
        -t,  0,  0.5,
        -t,  0,  0.5
    };

    float uv_coords[] =
    {
      0.072405, 0.144686,
      0.27133,  0.10033,
      0.481007, 0.129901,
      0.681948, 0.115116,
      0.928587, 0.107723,
      0.29888,  0.70651,
      0.449421, 0.736084,
      0.108695, 0.129901,
      0.847270, 0.699121,
      0.868776, 0.728691,
      0.576437, 0.721299,
      0.319044, 0.750869,
      0.548883, 0.736084,
      0.388265, 0.115116,
      0.397674, 0.713906,
      0.149690, 0.122508,
      0.347942, 0.115116,
      0.583157, 0.107723,
      0.780738, 0.100331,
      0.229663, 0.107723,
      0.33436,  0.68882,
      0.181276, 0.721299,
      0.379528, 0.721299,
      0.190684, 0.122508,
      0.358023, 0.736084,
      0.103319, 0.713906,
      0.24512,  0.70651,
      0.083157, 0.699121,
      0.31006,  0.10251,
      0.222942, 0.721299,
      0.772001, 0.728691,
      0.618776, 0.721299,
      0.745120, 0.721299,
      0.433292, 0.115116,
      0.598614, 0.713906,
      0.520657, 0.721299,
      0.650362, 0.736084,
      0.700765, 0.728691,
      0.878184, 0.107723,
      0.671867, 0.743476,
      0.914475, 0.743476,
      0.819717, 0.736084,
      0.201437, 0.721299,
      0.633561, 0.122508,
      0.796195, 0.743476,
      0.140953, 0.728691,
      0.122136, 0.706514,
      0.262593, 0.721299,
      0.157754, 0.728691,
      0.825765, 0.115116,
      0.723614, 0.728691,
      0.897001, 0.721299,
      0.499152, 0.736084,
      0.731007, 0.115116,
      0.469582, 0.721299,
      0.045523, 0.691729,
      0.967566, 0.721299,
      0.28341,  0.67157,
      0.532754, 0.122508,
      0.418507, 0.706514,
    };

    int num_indices = num_faces * 3;
    unsigned short faces[] = {
        // ... and 20 triangular faces, 
        // defined by the following vertex indices:
        0,  55, 25,
        1,  26,  5,
        2,  6, 35,
        3,  36, 50,
        4,  51, 56,
        7,  27, 45,
        28, 57, 20,
        58, 52, 10,
        53, 37, 30,
        38,  8, 40,
        15, 46, 21,
        16, 22, 11,
        17, 12, 31,
        18, 32, 41,
        19, 42, 47,
        23, 48, 29,
        13, 24, 59,
        33, 14, 54,
        43, 34, 39,
        49, 44, 9
    };

    glm::vec4 face_normals[60];

    float p1[3], p2[3], p3[3];

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    int w_width = 800;
    int w_height = 600;
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello NotCube", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Setting up the normals---------------------------------------------------------------------------------
    glm::vec4 normal;
    for (int i = 0; i < 20; i++) {

        // Assigning the points to p1, p2, p3
        p1[0] = points[faces[i * 3] * 3];
        p1[1] = points[faces[i * 3] * 3 + 1];
        p1[2] = points[faces[i * 3] * 3 + 2];

        p2[0] = points[faces[i * 3 + 1] * 3];
        p2[1] = points[faces[i * 3 + 1] * 3 + 1];
        p2[2] = points[faces[i * 3 + 1] * 3 + 2];

        p3[0] = points[faces[i * 3 + 2] * 3];
        p3[1] = points[faces[i * 3 + 2] * 3 + 1];
        p3[2] = points[faces[i * 3 + 2] * 3 + 2];

        // Calculating the normal for the triangle
        float p2mp1[3], p3mp1[3];
        subArrays(p2mp1, p1, p2);
        subArrays(p3mp1, p1, p3);
        glm::vec3 P2MP1 = glm::vec3(p2mp1[0], p2mp1[1], p2mp1[2]);
        glm::vec3 P3MP1 = glm::vec3(p3mp1[0], p3mp1[1], p3mp1[2]);

        
        normal = glm::vec4(glm::cross(P2MP1, P3MP1), 0.0f);

        face_normals[faces[i * 3]] = normal;
        face_normals[faces[i * 3 + 1]] = normal;
        face_normals[faces[i * 3 + 2]] = normal;
    };

    for (int i = 0; i < 60; i++) {
        printf("Normals: %f, %f, %f\n", float(face_normals[i].x), float(face_normals[i].y), float(face_normals[i].z));
    };


    //--------------------------------------------------------------------------------------------------------

    GLuint VAO, VBO, EBO, normalVBO, uv_coordinates;
    // Setup VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Setup VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // The new vbo for normals setup
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(face_normals), &face_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // VBO for uv coordinates
    glGenBuffers(1, &uv_coordinates);
    glBindBuffer(GL_ARRAY_BUFFER, uv_coordinates);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), &uv_coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // Setup EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 60 * sizeof(float), &faces, GL_STATIC_DRAW);

    loadShaders(vertex_shader_str, fragment_shader_str);

    // Generating OpenGL texture object
    GLuint texture_handle1;
    glGenTextures(1, &texture_handle1);  // Bug here, says i want to access 0x0 so have to rebuild every time before run
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader_program, "tex_sampler"), 0);
    glBindTexture(GL_TEXTURE_2D, texture_handle1);

    // Generating OpenGL texture object
    GLuint environtment_texture;
    glGenTextures(1, &environtment_texture);
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader_program, "env_sampler"), 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environtment_texture);

    // Loading images to environment texture
    unsigned char* image_data_front;
    unsigned char* image_data_back;
    unsigned char* image_data_right;
    unsigned char* image_data_left;
    unsigned char* image_data_up;
    unsigned char* image_data_down;

    unsigned image_w_front, image_w_back, image_w_right, image_w_left, image_w_up, image_w_down;
    unsigned image_h_front, image_h_back, image_h_right, image_h_left, image_h_up, image_h_down;

    unsigned err_code_F = lodepng_decode32_file(&image_data_front, &image_w_front, &image_h_front, "../../common/data/_front.png");
    if (!err_code_F)
        std::cout << "Read " << image_h_front << " x " << image_w_front << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image_w_front, image_h_front, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_front);

    unsigned err_code_B = lodepng_decode32_file(&image_data_back, &image_w_back, &image_h_back, "../../common/data/_back.png");
    if (!err_code_B)
        std::cout << "Read " << image_h_back << " x " << image_w_back << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image_w_back, image_h_back, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_back);

    unsigned err_code_R = lodepng_decode32_file(&image_data_right, &image_w_right, &image_h_right, "../../common/data/_right.png");
    if (!err_code_R)
        std::cout << "Read " << image_h_right << " x " << image_w_right << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image_w_right, image_h_right, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_right);

    unsigned err_code_L = lodepng_decode32_file(&image_data_left, &image_w_left, &image_h_left, "../../common/data/_left.png");
    if (!err_code_L)
        std::cout << "Read " << image_h_left << " x " << image_w_left << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image_w_left, image_h_left, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_left);

    unsigned err_code_U = lodepng_decode32_file(&image_data_up, &image_w_up, &image_h_up, "../../common/data/_up.png");
    if (!err_code_U)
        std::cout << "Read " << image_h_up << " x " << image_w_up << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image_w_up, image_h_up, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_up);

    unsigned err_code_D = lodepng_decode32_file(&image_data_down, &image_w_down, &image_h_down, "../../common/data/_down.png");
    if (!err_code_D)
        std::cout << "Read " << image_h_down << " x " << image_w_down << " image\n";
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image_w_down, image_h_down, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_down);

    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

        int shader_location = glGetUniformLocation(shader_program, "mvpMatrix");
        glUniformMatrix4fv(shader_location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
        int modViewMat_location = glGetUniformLocation(shader_program, "model_view_matrix");
        glUniformMatrix4fv(modViewMat_location, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        // -----------------------------------------------------------------------//
        // Setup light values for the fragment shader
        // Number of lights
        glm::uint numberOfLights(lights);
        int light_nr_location = glGetUniformLocation(shader_program, "light_count");
        glUniform1i(light_nr_location, numberOfLights);

        // Light position (infront of the object, no other way to get close to fig3 in LAB2.pdf)
        glm::vec3 lightPosition = { 1.0, 1.0, 1.0 };
        int light_position_location = glGetUniformLocation(shader_program, "light_position");
        glUniform3fv(light_position_location, 1, glm::value_ptr(lightPosition));

        // Light color
        glm::vec3 lightColor = { 1.0, 1.0, 1.0 };
        int light_color_location = glGetUniformLocation(shader_program, "light_colour");
        glUniform3fv(light_color_location, 1, glm::value_ptr(lightColor));
        // -----------------------------------------------------------------------//

        //-----------------------------------------------------------------------//
        // Call glDrawElements as before
        glClearColor(75.0f / 255.0f, 120.0f / 255.0f, 238.0f / 255.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, 0);
        //-----------------------------------------------------------------------//

        glfwSwapBuffers(window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
};

