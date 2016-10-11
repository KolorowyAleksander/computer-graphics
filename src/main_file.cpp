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
#include <tiny_obj_loader.h>

#include <iostream>

#include "shaderprogram.h"
#include "allmodels.h"
#include "main_file.h"
#include "Camera.h"

using namespace glm;

//Shader program object
ShaderProgram *shaderProgram;

//VAO and VBO handles
GLuint vao;
GLuint bufferVertices; //handle for VBO buffer which stores vertex coordinates
GLuint bufferColors;  //handle for VBO buffer which stores vertex colors
GLuint bufferNormals; //handle for VBO buffer which stores vertex normals
GLuint bufferTextureCoordinates;

GLuint texture;
GLuint textureShine;

int vertexCount = Models::CubeInternal::vertexCount;

std::vector<std::vector<int>> hardcodedMaze = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

std::vector<glm::mat4> cubeModelVectors = calculateMaze(hardcodedMaze);
std::vector<glm::mat4> floorModelVectors = calculateFloor(hardcodedMaze);

//Error handling procedure
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

//Initialization procedure
void initOpenGLProgram(GLFWwindow *window) {
  //************Insert initialization code here************

//  tinyobj::attrib_t attribs;
//  std::vector<tinyobj::shape_t> shapes;
//  std::vector<tinyobj::material_t> material;
//  std::string err;
//
//  if (!tinyobj::LoadObj(&attribs, &shapes, &material, &err, "wall.obj")) {
//    exit(1);
//  } else if (!err.empty()) {
//    std::cout << err;
//    //exit(1);
//  }

  //set cursor position to the middle of the window
  int xpos, ypos;
  glfwGetWindowSize(window, &xpos, &ypos);
  glfwSetCursorPos(window, (double) xpos / 2, (double) ypos / 2);

  //disable mouse cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  glClearColor(0, 0, 0, 1); //Clear the screen to black
  glEnable(GL_DEPTH_TEST); //Turn on Z-Buffer
  glfwSetKeyCallback(window, Camera::key_callback); //Register key event processing procedure

  std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading Language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  shaderProgram = new ShaderProgram("vshader.glsl", NULL, "fshader.glsl"); //Read, compile and link the shader program
  texture = readTexture("brickwall.png");
  textureShine = readTexture("brickwall_contrast.png");

  //*****Preparation for drawing of a single object*******

  Models::Cube cube = Models::Cube();
  bufferVertices = makeBuffer(cube.vertices, cube.vertexCount, sizeof(float) * 4); //VBO with vertex coordinates
  bufferColors = makeBuffer(cube.colors, cube.vertexCount, sizeof(float) * 4);//VBO with vertes colors
  bufferNormals = makeBuffer(cube.normals, cube.vertexCount, sizeof(float) * 4);
  bufferTextureCoordinates = makeBuffer(cube.texCoords, cube.vertexCount, sizeof(float) * 2);

  //Create VAO which associates VBO with attributes in shading program
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  assignVBOtoAttribute(shaderProgram, "vertex", bufferVertices, 4);
  assignVBOtoAttribute(shaderProgram, "color", bufferColors, 4);
  assignVBOtoAttribute(shaderProgram, "normal", bufferNormals, 4);
  assignVBOtoAttribute(shaderProgram, "inTextureCoordinates", bufferTextureCoordinates, 2);

  glBindVertexArray(0); //Deactivate VAO
  //******End of object preparation************
}

//Freeing of resources
void freeOpenGLProgram() {
  delete shaderProgram; //Delete shader program

  glDeleteVertexArrays(1, &vao); //Delete VAO
  //Delete VBOs
  glDeleteBuffers(1, &bufferVertices);
  glDeleteBuffers(1, &bufferColors);
  glDeleteBuffers(1, &bufferNormals);
  glDeleteBuffers(1, &bufferTextureCoordinates);

  //cleanup of singletons
  delete Camera::getInstance();
  delete Settings::getInstance();
}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {
  //Turn on the shading program that will be used for drawing. We use only one, so we do it only once.
  shaderProgram->use();

  glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));

  glUniform1i(shaderProgram->getUniformLocation("textureMap"), 0);
  glUniform1i(shaderProgram->getUniformLocation("textureShineMap"), 0);
  glUniform1f(shaderProgram->getUniformLocation("cutoffAngle"), glm::cos(glm::radians(12.5f)));

  glUniform4f(
      shaderProgram->getUniformLocation("lightDirection"),
      Camera::getInstance()->getDirection().x,
      Camera::getInstance()->getDirection().y,
      Camera::getInstance()->getDirection().z,
      1
  );

  glUniform4f(
      shaderProgram->getUniformLocation("lightPosition"),
      Camera::getInstance()->getPosition().x,
      Camera::getInstance()->getPosition().y,
      Camera::getInstance()->getPosition().z,
      1
  );

  //Activation of VAO and therefore making all associations of VBOs and attributes current
  glBindVertexArray(vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureShine);

  //Drawing of an object
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  //Tidying up after ourselves (not needed if we use VAO for every object)
  glBindVertexArray(0);
}

//Procedure which draws the scene
void drawScene(GLFWwindow *window) {
  //************Place the drawing code here******************

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

  glm::mat4 P = Camera::getInstance()->getPerspectiveMatrix();
  glm::mat4 V = Camera::getInstance()->getVievMatrix(); //get P and V matrices from camera singleton

  for(auto matrix : cubeModelVectors) {
    drawObject(vao, shaderProgram, P, V, matrix);
  }

  for(auto matrix : floorModelVectors) {
    drawObject(vao, shaderProgram, P, V, matrix);
  }
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

  std::cout << Settings::getInstance()->getWindowHeight() << ": height" << std::endl;
  std::cout << Settings::getInstance()->getWindowWidth() << ": width" << std::endl;

  window = glfwCreateWindow(Settings::getInstance()->getWindowWidth(),
                            Settings::getInstance()->getWindowHeight(),
                            "OpenGL",
                            Settings::getInstance()->getMonitorOrNull(),
                            NULL);  //Create window with "OpenGL" as well as OpenGL context.


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
    Camera::getInstance()->computeCamera(window, (float) deltaTime, cubeModelVectors);
    glfwSetTime(0); //Zero time counter
    drawScene(window); //Execute drawing procedure
    glfwPollEvents(); //Execute callback procedures which process events

  }

  freeOpenGLProgram(); //Free resources

  glfwDestroyWindow(window); //Delete OpenGL context and window
  glfwTerminate(); //Free GLFW resources
  exit(EXIT_SUCCESS);
}
