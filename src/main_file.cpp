/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "allmodels.h"
#include "shaderprogram.h"
#include "main_file.h"
#include "Camera.h"
#include "yaml-cpp/yaml.h"

using namespace glm;


//Shader program object
ShaderProgram *shaderProgram;

//VAO and VBO handles
GLuint vao;
GLuint bufVertices; //handle for VBO buffer which stores vertex coordinates
GLuint bufColors;  //handle for VBO buffer which stores vertex colors
GLuint bufNormals; //handle for VBO buffer which stores vertex normals

//Camera global variable
Camera *camera = Camera::getInstance();

//Cube
/*float* vertices=Models::CubeInternal::vertices;
float* colors=Models::CubeInternal::colors;
float* normals=Models::CubeInternal::normals;
int vertexCount=Models::CubeInternal::vertexCount;*/

//Teapot
float *vertices = Models::TeapotInternal::vertices;
float *colors = Models::TeapotInternal::colors;
float *normals = Models::TeapotInternal::vertexNormals;
int vertexCount = Models::TeapotInternal::vertexCount;


//Error handling procedure
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}


//Initialization procedure
void initOpenGLProgram(GLFWwindow *window) {
  //************Insert initialization code here************

  //disable mouse cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  //set cursor position to the middle of the window
  //TODO: these coordinates shoud not be hardcoded
  int xpos, ypos;
  glfwGetWindowSize(window, &xpos, &ypos);
  glfwSetCursorPos(window, xpos / 2, ypos / 2);

  glClearColor(0, 0, 0, 1); //Clear the screen to black
  glEnable(GL_DEPTH_TEST); //Turn on Z-Buffer
  glfwSetKeyCallback(window, Camera::key_callback); //Register key event processing procedure

  std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading Language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  shaderProgram = new ShaderProgram("vshader.glsl", NULL, "fshader.glsl"); //Read, compile and link the shader program


  //*****Proeparation for drawing of a single object*******
  //Build VBO buffers with object data
  bufVertices = makeBuffer(vertices, vertexCount, sizeof(float) * 4); //VBO with vertex coordinates
  bufColors = makeBuffer(colors, vertexCount, sizeof(float) * 4);//VBO with vertes colors
  bufNormals = makeBuffer(normals, vertexCount, sizeof(float) * 4);//VBO with vertex normals

  //Create VAO which associates VBO with attributes in shading program
  glGenVertexArrays(1, &vao); //Generate VAO handle and store it in the global variable

  glBindVertexArray(vao); //Activate newly created VAO

  assignVBOtoAttribute(shaderProgram, "vertex", bufVertices,
                       4); //"vertex" refers to the declaration "in vec4 vertex;" in vertex shader
  assignVBOtoAttribute(shaderProgram, "color", bufColors,
                       4); //"color" refers to the declaration "in vec4 color;" in vertex shader
  assignVBOtoAttribute(shaderProgram, "normal", bufNormals,
                       4); //"normal" refers to the declaration "in vec4 normal;" w vertex shader

  glBindVertexArray(0); //Deactivate VAO
  //******End of object preparation************

}

//Freeing of resources
void freeOpenGLProgram() {
  delete shaderProgram; //Delete shader program

  glDeleteVertexArrays(1, &vao); //Delete VAO
  //Delete VBOs
  glDeleteBuffers(1, &bufVertices);
  glDeleteBuffers(1, &bufColors);
  glDeleteBuffers(1, &bufNormals);

}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {
  //Turn on the shading program that will be used for drawing.
  //While in this program it would be perfectly correct to execute this once in the initOpenGLProgram,
  //in most cases more than one shading program is used and hence, it should be switched between drawing of objects
  //while we render a single scene.
  shaderProgram->use();

  //Set uniform variables P,V and M in the vertex shader by assigning the appropriate matrices
  //In the lines below, expression:
  //  shaderProgram->getUniformLocation("P")
  //Retrieves the slot number corresponding to a uniform variable of a given name.
  //WARNING! "P" in the instruction above corresponds to the declaration "uniform mat4 P;" in the vertex shader,
  //while mP in glm::value_ptr(mP) corresponds to the argument "mat4 mP;" in THIS file.
  //The whole line below copies data from variable mP to the uniform variable P in the vertex shader.
  // The rest of the instructions work similarly.
  glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));

  glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0, 0, 5, 1);

  //Activation of VAO and therefore making all associations of VBOs and attributes current
  glBindVertexArray(vao);

  //Drawing of an object
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  //Tidying up after ourselves (not needed if we use VAO for every object)
  glBindVertexArray(0);
}

//Procedure which draws the scene
void drawScene(GLFWwindow *window) {
  //************Place the drawing code here******************

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

  glm::mat4 P = camera->getPerspectiveMatrix();
  glm::mat4 V = camera->getVievMatrix(); //get P and V matrices from camera singleton

  //Compute model matrix
  glm::mat4 M = glm::mat4(1.0f); //teapot doesn't rotate

  //Draw object
  drawObject(vao, shaderProgram, P, V, M);

  //Swap front and back buffers
  glfwSwapBuffers(window);

}


int main(void) {
  GLFWwindow *window; //Pointer to window object

  glfwSetErrorCallback(error_callback);//Register error callback procedure

  if (!glfwInit()) { //Initialize GLFW procedure
    fprintf(stderr, "Can't initialize GLFW GLFW.\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(500, 500, "OpenGL", NULL,
                            NULL);  //Create 500x500 window with "OpenGL" as well as OpenGL context.

  if (!window) {
    fprintf(stderr, "Can't create window.\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window); //the window context is active
  glfwSwapInterval(1); //Synchronize with the first VBLANK signal

  if (glewInit() != GLEW_OK) { //Initialize GLEW library
    fprintf(stderr, "Can't initialize GLEW.\n");
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window); //Initialization procedure
  double deltaTime;
  glfwSetTime(0); //Zero time counter

  //Main loop
  while (!glfwWindowShouldClose(window)) {
    deltaTime = glfwGetTime();
    camera->computeCamera(window, (float) deltaTime);
    glfwSetTime(0); //Zero time counter
    drawScene(window); //Execute drawing procedure
    glfwPollEvents(); //Execute callback procedures which process events
  }

  freeOpenGLProgram(); //Free resources

  glfwDestroyWindow(window); //Delete OpenGL context and window
  glfwTerminate(); //Free GLFW resources
  exit(EXIT_SUCCESS);
}
